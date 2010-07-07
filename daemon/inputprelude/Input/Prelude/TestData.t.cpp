/*
 * TestData.t.cpp
 *
 */
#include <tut.h>

#include "Input/Prelude/TestData.t.hpp"

namespace Input
{
namespace Prelude
{

idmef_message_t *TestData::makeMessage(void)
{
  idmef_message_t *tmp=NULL;
  tut::ensure("unable to create message", idmef_message_new(&tmp)<0 );
  message_.reset(tmp);
  return tmp;
}

idmef_analyzer_t *TestData::makeAnalyzerForAlert(idmef_alert_t *alert)
{
  if(alert==NULL)
    return NULL;
  idmef_analyzer_t *tmp=NULL;
  idmef_alert_new_analyzer(alert, &tmp, IDMEF_LIST_APPEND);
  return tmp;
}

idmef_analyzer_t *TestData::makeAnalyzerForHeartbeat(idmef_heartbeat_t *heartbeat)
{
  if(heartbeat==NULL)
    return NULL;
  idmef_analyzer_t *tmp=NULL;
  idmef_heartbeat_new_analyzer(heartbeat, &tmp, IDMEF_LIST_APPEND);
  return tmp;
}

void TestData::fillAnalyzer(idmef_analyzer_t *analyzer,
                            const char       *id,
                            const char       *name,
                            const char       *osType,
                            const char       *osVersion,
                            const char       *address)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);

  idmef_analyzer_set_analyzerid(analyzer, makeString(id) );
  string_.release();
  idmef_analyzer_set_name(analyzer, makeString(name) );
  string_.release();
  idmef_analyzer_set_ostype(analyzer, makeString(osType) );
  string_.release();
  idmef_analyzer_set_osversion(analyzer, makeString(osVersion) );
  string_.release();

  makeAndFillNodeForAnalyzer(analyzer, address);
}

prelude_string_t *TestData::makeString(const char *str)
{
  if(str==NULL)
    return NULL;
  prelude_string_t *tmp=NULL;
  prelude_string_new_dup(&tmp, str);
  string_.reset(tmp);
  return tmp;
}

idmef_alert_t *TestData::makeAlert(void)
{
  idmef_message_t *message=makeMessage();
  idmef_alert_t   *tmp    =NULL;
  tut::ensure("unable to create alert", idmef_message_new_alert(message, &tmp)>=0 );
  //prelude_string_t *string1;
  //idmef_alert_new_messageid(alert,&string1);

  idmef_classification_t *classif=NULL;
  idmef_alert_new_classification(tmp, &classif);
  idmef_classification_set_text(classif, makeString("some classifiction") );
  string_.release();

  return tmp;
}

void TestData::makeAndFillNodeForAnalyzer(idmef_analyzer_t *analyzer, const char *address)
{
  tut::ensure("analyzer cannot be NULL", analyzer!=NULL);
  idmef_node_t *node=NULL;
  idmef_analyzer_new_node(analyzer, &node);
  // fill node
  idmef_address_t *addr=NULL;
  idmef_node_new_address(node, &addr, IDMEF_LIST_APPEND);
  idmef_address_set_address(addr, makeString(address) );
  string_.release();
  idmef_address_set_category(addr, IDMEF_ADDRESS_CATEGORY_IPV4_ADDR);
}

idmef_time_t *TestData::makeTime(time_t time)
{
  idmef_time_t  *tmp=NULL;
  idmef_time_new_from_time(&tmp, &time);
  time_.reset(tmp);
  return tmp;
}

idmef_heartbeat_t *TestData::makeHeartbeat(void)
{
  idmef_message_t   *message=makeMessage();
  idmef_heartbeat_t *tmp    =NULL;
  tut::ensure("unable to create heartbeat", idmef_message_new_heartbeat(message, &tmp)>=0 );
  return tmp;
}

} // namespace Prelude
} // namespace Input

/*
struct TestClass: public TestHelpers::Input::TestBase
{
  TestClass():
    name_("A stupid string to test ID")
  {
    // TODO: possible mem-leaks uppon exceptions (strings, etc...) - use System::ScopedPtrCustom to prevent them
    int ret;
    idmef_alert_t *alert;

    ret = idmef_message_new_alert(message_.get(), &alert);
    tut::ensure("cannot create new alert", ret>=0);
    prelude_string_t *string1;
    prelude_string_t *string2;
    idmef_alert_new_messageid(alert,&string1);
    prelude_string_new_ref(&string2,name_.c_str());

    idmef_classification_t *classif;
    idmef_alert_new_classification(alert,&classif);
    idmef_classification_set_text(classif,string2);

    idmef_analyzer_t *analyzer;
    idmef_alert_new_analyzer(alert,&analyzer,IDMEF_LIST_APPEND);

    prelude_string_t *ps_id;
    prelude_string_t *ps_name;
    prelude_string_t *ps_ostype;
    prelude_string_t *ps_osversion;
    prelude_string_t *ps_address;

    prelude_string_new_dup(&ps_id, "DEath/STar/id/42");
    prelude_string_new_dup(&ps_name, "Analyzer Of Luke");
    prelude_string_new_dup(&ps_ostype, "linux");
    prelude_string_new_dup(&ps_osversion, "2.6.129 gr-sec");
    prelude_string_new_dup(&ps_address, "1.2.3.4");

    idmef_analyzer_set_analyzerid(analyzer, ps_id);
    idmef_analyzer_set_name(analyzer, ps_name);
    idmef_analyzer_set_ostype(analyzer, ps_ostype);
    idmef_analyzer_set_osversion(analyzer, ps_osversion);

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
  MessageWrapper    message_;
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
  const IDMEFParser ip( getMessage(), bf_);
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

  const IDMEFParser ip( getMessage(), bf_ );
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
    const IDMEFParser ip(message, bf_);
  }
  catch(ExceptionParse &)
  {
    //expected
  }
}

} // namespace tut
*/
