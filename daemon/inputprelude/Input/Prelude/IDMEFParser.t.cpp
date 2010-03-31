/*
 * IDMEFParser.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include "Input/Exception.hpp"
#include "Input/Prelude/IDMEFParser.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;

namespace
{

struct TestClass
{
  TestClass():name_("A stupid string to test ID")
  {
    int ret;
    idmef_alert_t *alert;

    ret = idmef_message_new(&message);
    if ( ret < 0 )
      throw Input::Exception(SYSTEM_SAVE_LOCATION,"Cannot create new IDMEF message.");

    ret = idmef_message_new_alert(message, &alert);
    if ( ret < 0 )
      throw Input::Exception(SYSTEM_SAVE_LOCATION,"Cannot create new ALERT.");

    prelude_string_t *string1,*string2;
    idmef_alert_new_messageid(alert,&string1);
    prelude_string_new_ref(&string2,name_.c_str());
    idmef_alert_set_messageid(alert,string2);
    
    idmef_analyzer_t *analyzer;
    idmef_alert_new_analyzer(alert,&analyzer,IDMEF_LIST_APPEND);
    prelude_string_t *ps_name,*ps_ostype,*ps_osversion,*ps_address;

    std::string name("The Analyzer of Luke Skywaker"),ostype("Wojtek linux"),osversion("2.6.129 gr-sec"),address("156.117.92.22");

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
  }

  idmef_message_t * getMessage()
  {
    return message;
  }

  idmef_alert_t * getAlert()
  {
    return idmef_message_get_alert(message);
  }

  const std::string & getName()
  {
    return name_;
  }

private:
  idmef_message_t *message;
  const std::string name_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/IDMEFParser");
} // unnamed namespace


namespace tut
{
template<>
template<>
void testObj::test<1>(void)
{
  IDMEFParser ip(getMessage());
  ensure(ip.getName()==Persistency::Alert::Name(getName()));
}

template<>
template<>
void testObj::test<2>(void)
{
  idmef_alert_t *alert=getAlert();

  const char * time_char="20020131T235959";
  Persistency::Timestamp time(from_iso_string(time_char));
  tm timeptr=to_tm(time);
  const time_t t=mktime(&timeptr);
  idmef_time_t *idmeftime;
  idmef_time_new_from_time(&idmeftime,&t);
  idmef_alert_set_create_time(alert,idmeftime);

  IDMEFParser ip(getMessage()); 
  ensure_equals("Something broken with time",string(to_iso_string(ip.getCreateTime()).c_str()),string(time_char)); 
}

} // namespace tut
