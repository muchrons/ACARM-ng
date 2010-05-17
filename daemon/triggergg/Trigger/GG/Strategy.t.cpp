/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( AccountConfig(1234, "secr3t"), 997, Trigger::Simple::ThresholdConfig("1.2", "3") )
  {
  }

  const Config cfg_;    // default config
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s(cfg_);
  ensure_equals("invalid name", s.getTriggerName(), "gg");
}

// 
template<>
template<>
void testObj::test<2>(void)
{
  // TODO
}

// 
template<>
template<>
void testObj::test<3>(void)
{
  // TODO
}

// 
template<>
template<>
void testObj::test<4>(void)
{
  // TODO
}

// 
template<>
template<>
void testObj::test<5>(void)
{
  // TODO
}

} // namespace tut
