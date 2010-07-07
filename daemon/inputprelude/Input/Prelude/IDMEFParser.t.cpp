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
#include "Input/Prelude/TestData.t.hpp"
#include "TestHelpers/Input/TestBase.hpp"

using namespace std;
using namespace Input::Prelude;
using namespace boost::posix_time;

namespace
{

struct TestClass: public TestHelpers::Input::TestBase
{
  TestData td_;
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
  td_.makeAlert();
  const IDMEFParser ip(td_.message_.get(), bf_);
  ensure_equals("invalid IP", ip.getName().get(), string("some classification") );
}

// test name description's missing
template<>
template<>
void testObj::test<2>(void)
{
  const time_t                  tt=54321;
  const Persistency::Timestamp  time(tt);

  idmef_alert_t *alert=td_.makeAlert();
  idmef_alert_set_create_time(alert, td_.makeTime(tt) );
  td_.time_.release();

  const IDMEFParser ip(td_.message_.get(), bf_);
  ensure_equals("Something broken with time", ip.getCreateTime(), time);
}

// test parsing when heart beat is passed
template<>
template<>
void testObj::test<3>(void)
{
  td_.makeHeartbeat();
  try
  {
    const IDMEFParser ip(td_.message_.get(), bf_);
    fail("parser didn't throw on unexpected message type - heartbeat");
  }
  catch(const ExceptionUnsupportedFeature &)
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
