/*
 * TimeoutChecker.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "TestHelpers/TimeoutChecker.hpp"

using namespace TestHelpers;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/TimeoutChecker");
} // unnamed namespace


namespace tut
{

// test if d-tor does nothing, even in case of timeout
template<>
template<>
void testObj::test<1>(void)
{
  const TimeoutChecker tc(0);
}

// test error on timeout
template<>
template<>
void testObj::test<2>(void)
{
  const TimeoutChecker tc(0);
  sleep(1);
  bool error=false;
  try
  {
    tc();       // should throw
    error=true;
  }
  catch(...)
  {
    // this is expected
  }
  if(error)
    fail("no exception thrown on timeout");
}

// test default return value, when no timeout
template<>
template<>
void testObj::test<3>(void)
{
  const TimeoutChecker tc(1234567);
  ensure("invalid default return value", tc() );
}

// test custom error message
template<>
template<>
void testObj::test<4>(void)
{
  const TimeoutChecker tc(1234567, "my test message");
  ensure("invalid default return value", tc() );
}

} // namespace tut
