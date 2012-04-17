/*
 * PeriodicQueriesThread.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "Core/PeriodicQueriesThread.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/PeriodicQueriesThread");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  PeriodicQueriesThread ct;
}

// check if sleeping is interruptible (stubbed cleanup will exit straight away)
template<>
template<>
void testObj::test<2>(void)
{
  PeriodicQueriesThread ct;
  usleep(50*1000);
}

} // namespace tut
