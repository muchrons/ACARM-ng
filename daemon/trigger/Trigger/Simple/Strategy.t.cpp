/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Simple/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Trigger::Simple;
using namespace TestHelpers::Persistency;

namespace
{

struct TestTrigger: public Strategy
{
  TestTrigger(const char *severity, const char *count):
    Strategy("testtrigger", ThresholdConfig(severity, count) ),
    callsTrigger_(0)
  {
  }

  virtual void trigger(const Persistency::GraphNodePtrNN &/*n*/)
  {
    ++callsTrigger_;
  }

  int callsTrigger_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  void check(TestTrigger &tt, const Persistency::GraphNodePtrNN n, const size_t cnt)
  {
    tut::ensure_equals("pre-condition failed", tt.callsTrigger_, 0);
    TestTrigger::ChangedNodes cn;
    tt.process(n, cn);
    tut::ensure_equals("trigger called invalid number of times", tt.callsTrigger_, cnt);
  }

};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Simple/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  TestTrigger tt(NULL, NULL);
  ensure_equals("invalid name", tt.getTriggerName(), "testtrigger");
}

// test trigger when alerts count is above threshold
template<>
template<>
void testObj::test<2>(void)
{
  TestTrigger tt(NULL, "2");
  check(tt, makeNewTree1(), 1);
}

// test trigger when severity is above threshold
template<>
template<>
void testObj::test<3>(void)
{
  TestTrigger tt("1.0", NULL);
  check(tt, makeNewTree1(), 1);
}

// test trigger when severity equals threshold
template<>
template<>
void testObj::test<4>(void)
{
  TestTrigger tt("1.1", NULL);
  check(tt, makeNewNode(), 1);
}

// test trigger when alerts count equals threshold
template<>
template<>
void testObj::test<5>(void)
{
  TestTrigger tt(NULL, "2");
  check(tt, makeNewNode(), 1);
}

// test if trigger is NOT called, when both values are below threshold
template<>
template<>
void testObj::test<6>(void)
{
  TestTrigger tt("666", "42");
  check(tt, makeNewLeaf(), 0);
}

// test if trigger is NOT called, when both thresholds are not set
template<>
template<>
void testObj::test<7>(void)
{
  TestTrigger tt(NULL, NULL);
  check(tt, makeNewLeaf(), 0);
}

} // namespace tut
