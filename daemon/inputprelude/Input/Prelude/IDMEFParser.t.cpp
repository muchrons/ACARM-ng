/*
 * IDMEFParser.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include "Input/Exception.hpp"
#include "Input/Prelude/ExceptionParse.hpp"
#include "Input/Prelude/IDMEFParser.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;

namespace
{

// TODO: this class must be non-copyable
class MessageWrapper
{
public:
  MessageWrapper()
  {
    if (idmef_message_new(&message_)<0)
      tut::fail("Unable to create message object.");
  }

  ~MessageWrapper()
  {
    idmef_message_destroy(message_);
  }

  idmef_message_t * get()
  {
    // TODO: add assert on message_!=NULL
    return message_;
  }

private:
  idmef_message_t *message_;
};



struct TestClass
{
  TestClass():
    name_("A stupid string to test ID")
  {
    // TODO: possible mem-leaks uppon exceptions (strings, etc...) - use System::ScopedPtrCustom to prevent them
    int ret;
    idmef_alert_t *alert;

    ret = idmef_message_new_alert(message_.get(), &alert);
    if ( ret < 0 )
      throw Input::Exception(SYSTEM_SAVE_LOCATION,"Cannot create new ALERT.");
    prelude_string_t *string1;
    prelude_string_t *string2;
    idmef_alert_new_messageid(alert,&string1);
    prelude_string_new_ref(&string2,name_.c_str());

    idmef_classification_t *classif;
    idmef_alert_new_classification(alert,&classif);
    idmef_classification_set_text(classif,string2);

    idmef_analyzer_t *analyzer;
    idmef_alert_new_analyzer(alert,&analyzer,IDMEF_LIST_APPEND);

    prelude_string_t *ps_name;
    prelude_string_t *ps_ostype;
    prelude_string_t *ps_osversion;
    prelude_string_t *ps_address;

    const std::string name("The Analyzer of Luke Skywaker");
    const std::string ostype("Wojtek linux");
    const std::string osversion("2.6.129 gr-sec");
    const std::string address("156.117.92.22");

    prelude_string_new_dup(&ps_name,name.c_str());
    prelude_string_new_dup(&ps_ostype,ostype.c_str());
    prelude_string_new_dup(&ps_osversion,osversion.c_str());
    prelude_string_new_dup(&ps_address,address.c_str());

    idmef_analyzer_set_name(analyzer,ps_name);
    idmef_analyzer_set_ostype(analyzer,ps_ostype);
    idmef_analyzer_set_osversion(analyzer,ps_osversion);

    idmef_node_t *node;
    idmef_analyzer_new_node(analyzer, &node);
    idmef_address_t * addr;

    idmef_node_new_address(node,&addr,IDMEF_LIST_APPEND);
    idmef_address_set_address(addr,ps_address);
    idmef_address_set_category(addr,IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
  }

  ~TestClass()
  {
  }

  idmef_message_t * getMessage()
  {
    return message_.get();
  }

  idmef_alert_t * getAlert()
  {
    return idmef_message_get_alert(message_.get());
  }

  const std::string & getName()
  {
    return name_;
  }

private:
  // TODO: fix indentation
  MessageWrapper message_;
  const std::string name_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParser");
} // unnamed namespace


namespace tut
{

// check if alert name is extracted properly
template<>
template<>
void testObj::test<1>(void)
{
  const IDMEFParser ip(getMessage());
  ensure_equals("IP",ip.getName().get(),getName());
}

// TODO: test name description's missing
template<>
template<>
void testObj::test<2>(void)
{
  const time_t                  tt=54321;
  const Persistency::Timestamp  time(tt);

  idmef_time_t  *idmeftime=NULL;
  idmef_alert_t *alert=getAlert();
  idmef_time_new_from_time(&idmeftime, &tt);

  idmef_alert_set_create_time(alert, idmeftime);

  const IDMEFParser ip( getMessage() );
  ensure_equals("Something broken with time", ip.getCreateTime(), time);
}

// TODO: test parsing when heart beat is passed
template<>
template<>
void testObj::test<3>(void)
{
  const time_t                  tt=54321;
  const Persistency::Timestamp  time(tt);

  MessageWrapper msg;
  idmef_message_t *message=msg.get();
  idmef_heartbeat_t *heartbeat;
  //prelude_string_t *str;

  if ( idmef_message_new_heartbeat(message, &heartbeat) < 0 )
    tut::fail("error creating new heartbeat");

  //if ( prelude_string_new_constant(&str, "Analyzer status") < 0 )
  //tut::fail("error creating new prelude_string");

  //add_hb_data(heartbeat, str, "some data");

  idmef_time_t  *idmeftime=NULL;

  idmef_time_new_from_time(&idmeftime, &tt);
  idmef_heartbeat_set_create_time(heartbeat, idmeftime);

  try
  {
    const IDMEFParser ip( message );
  }
  catch(ExceptionParse &)
  {
    //expected
  }
}


// TODO: test alert without analyzer

// TODO: test no source hosts

// TODO: test multiple source hosts

// TODO: test no target hosts

// TODO: test multiple target hosts

// TODO: test when no services are prensent in alert (this was a bug in previous versions)

// TODO: test when no processes are prensent in alert (this was a bug in previous versions)

} // namespace tut
