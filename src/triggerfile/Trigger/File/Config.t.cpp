/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/File/Config.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::File;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( "/tmp", Trigger::Simple::ThresholdConfig("1.2", "4") )
  {
  }

  const Config cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/File/Config");
} // unnamed namespace


namespace tut
{

// test getting path
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid path", cfg_.getOutputDirectory(), "/tmp");
}

// get severity threshold
template<>
template<>
void testObj::test<2>(void)
{
  ensure("severity threshold is NULL", cfg_.getThresholdConfig().getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity threshold", *cfg_.getThresholdConfig().getSeverityThreshold().get(), 1.2);

  ensure("alerts' count threshold is NULL", cfg_.getThresholdConfig().getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid alerts' count threshold", *cfg_.getThresholdConfig().getAlertCountThreshold().get(), 4);
}

// test copyability
template<>
template<>
void testObj::test<3>(void)
{
  const Config tmp=cfg_;
}

} // namespace tut
