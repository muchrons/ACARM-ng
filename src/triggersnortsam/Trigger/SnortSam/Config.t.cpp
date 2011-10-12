/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/Config.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::SnortSam;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( "host.net", 4242, "$3cr37", Who::SRC, How::IN|How::OUT, 10,
          Trigger::Simple::ThresholdConfig("1.2", "4") )
  {
  }

  const Config cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Config");
} // unnamed namespace


namespace tut
{

// get severity threshold
template<>
template<>
void testObj::test<1>(void)
{
  ensure("severity threshold is NULL", cfg_.getThresholdConfig().getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity threshold", *cfg_.getThresholdConfig().getSeverityThreshold().get(), 1.2);

  ensure("alerts' count threshold is NULL", cfg_.getThresholdConfig().getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid alerts' count threshold", *cfg_.getThresholdConfig().getAlertCountThreshold().get(), 4);
}

// test copyability
template<>
template<>
void testObj::test<2>(void)
{
  const Config tmp=cfg_;
}

// test host name
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid host", cfg_.getHost(), "host.net");
}

// test port number
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid port", cfg_.getPort().get(), 4242);
}

// test key
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid key", cfg_.getKey(), "$3cr37");
}

// test who
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("invalid who", cfg_.getWho().toInt(), Who::SRC);
}

// test how
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("invalid how", cfg_.getHow().toInt(), (How::IN|How::OUT).toInt() );
}

// test duration
template<>
template<>
void testObj::test<8>(void)
{
  ensure_equals("invalid duration", cfg_.getDuration(), 10);
}

} // namespace tut
