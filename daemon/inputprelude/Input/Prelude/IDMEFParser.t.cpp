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
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();

  const IDMEFParser ip(td_.message_.get(), bf_);
  ensure_equals("invalid IP", ip.getName().get(), string("some classification") );
}

// test alert create time
template<>
template<>
void testObj::test<2>(void)
{
  const time_t                  tt=54321;
  const Persistency::Timestamp  time(tt);

  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();
  td_.addTimeToAlert(tt);

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

// test alert without analyzer
template<>
template<>
void testObj::test<4>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");

  try
    {
      const IDMEFParser ip(td_.message_.get(), bf_);
    }
  catch(const ExceptionParse &)
  {
    //expected
  }
}

// test alert without classification
template<>
template<>
void testObj::test<5>(void)
{
  td_.makeAlert();
  td_.addAnalyzerToAlert();

  try
    {
      const IDMEFParser ip(td_.message_.get(), bf_);
    }
  catch(const ExceptionParse &)
  {
    //expected
  }
}

// multiple source hosts
template<>
template<>
void testObj::test<6>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();
  {
    idmef_source_t * src=td_.addSourceToAlert();
    td_.addAddressv4ToSource(src,"192.168.1.1");
  }
  {
    idmef_source_t * src=td_.addSourceToAlert();
    td_.addAddressv6ToSource(src,"::1");
  }
  const IDMEFParser ip(td_.message_.get(), bf_);
}

// multiple target hosts
template<>
template<>
void testObj::test<7>(void)
{
  td_.makeAlert();
  td_.addClassificationToAlert("some classification");
  td_.addAnalyzerToAlert();
  {
    idmef_target_t * src=td_.addTargetToAlert();
    td_.addAddressv4ToTarget(src,"192.168.1.1");
  }
  {
    idmef_target_t * src=td_.addTargetToAlert();
    td_.addAddressv6ToTarget(src,"::1");
  }
  const IDMEFParser ip(td_.message_.get(), bf_);
}


// TODO: test when no services are prensent in alert (this was a bug in previous versions) 
// There can be no service, everything is fine. This test is moved to IDMEFParserSource/Target

// TODO: test when no processes are prensent in alert (this was a bug in previous versions)
// The same as with service

} // namespace tut
