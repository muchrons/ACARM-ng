/*
 * CleanupThread.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "Core/CleanupThread.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/CleanupThread");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  CleanupThread ct;
}

// check if sleeping is interruptible (stubbed cleanup will exit straight away)
template<>
template<>
void testObj::test<2>(void)
{
  CleanupThread ct;
  usleep(50*1000);
}

} // namespace tut
