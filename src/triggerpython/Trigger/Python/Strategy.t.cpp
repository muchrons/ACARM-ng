/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Python/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Python;
using namespace Persistency;
using namespace TestHelpers::Persistency;
using namespace Core::Types::Proc;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  Config mkConfig(const Config::Path &script="testdata/do_nothing.py") const
  {
    const Trigger::Simple::ThresholdConfig th("1", "2");
    return Config(th, script);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Python/Strategy");
} // unnamed namespace


namespace tut
{

// check if trigger type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s(InstanceName("mysnaketrigger"), mkConfig());
  ensure_equals("invalid trigger type", s.getTriggerType().str(), "python");
}

// test forwarding call to python code
template<>
template<>
void testObj::test<2>(void)
{
  // TODO
}

} // namespace tut
