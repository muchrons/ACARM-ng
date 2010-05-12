/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Trigger/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
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
  ensure_equals("invalid name", tt_.getTriggerName(), "gg");
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
