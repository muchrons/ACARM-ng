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
    prelude_string_new_ref(&string2,name_);
    idmef_alert_set_messageid(alert,string2);
  }

  idmef_message_t * getMessage()
  {
    return message;
  }

  idmef_alert_t * getAlert()
  {
    return idmef_message_get_alert(message);
  }

  const char * getName()
  {
    return name_;
  }

private:
  idmef_message_t *message;
  const char *name_;
};

typedef TestClass TestClass;
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
  TestClass   tc;
  IDMEFParser ip(tc.getMessage());
  ensure(ip.getName()==Persistency::detail::LimitedNULLString<64>(tc.getName()));
}

template<>
template<>
void testObj::test<2>(void)
{
  TestClass tc;

  idmef_alert_t *alert=tc.getAlert();

  const char * time_char="2002-01-20 23:59:59.000";
  Persistency::Timestamp time(time_from_string(time_char));
  tm timeptr=to_tm(time);
  const time_t t=mktime(&timeptr);
  idmef_time_t *idmeftime;
  idmef_time_new_from_time(&idmeftime,&t);
  idmef_alert_set_create_time(alert,idmeftime);

  IDMEFParser ip(tc.getMessage());
  ensure(strcmp(to_simple_string(ip.getCreateTime()).c_str(),time_char)==0);
}

} // namespace tut
