/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Trigger/ExtApp/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::ExtApp;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    th_("1.2", "2"),
    cfg_("/bin/true", th_)
  {
  }

  const Trigger::Simple::ThresholdConfig th_;
  const Config                           cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/ExtApp/Strategy");
} // unnamed namespace


namespace tut
{

// check if type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s("myscriptrunner", cfg_);
  ensure_equals("invalid type", s.getTriggerType(), "extapp");
}

// test executing report
template<>
template<>
void testObj::test<2>(void)
{
  Strategy               s("myrunner", cfg_);
  Strategy::ChangedNodes nc;
  s.process( makeNewNode(), nc );
  // TODO
  fail("TODO");
}

} // namespace tut
