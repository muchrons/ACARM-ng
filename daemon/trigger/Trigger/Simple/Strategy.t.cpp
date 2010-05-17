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
  TestTrigger(void):
    Strategy("testtrigger"),
    callsTrigger_(0)
  {
  }

  virtual void trigger(const NodeType &/*n*/)
  {
    ++callsTrigger_;
  }

  int callsTrigger_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestTrigger tt_;
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
  ensure_equals("invalid name", tt_.getTriggerName(), "testtrigger");
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
