/*
 * ErrorThrower.t.cpp
 *
 */
#include <tut.h>

#include "DataFacades/StrAccess/ErrorThrower.hpp"
#include "DataFacades/StrAccess/DefaultHandleMap.hpp"
#include "DataFacades/StrAccess/TestParams.t.hpp"

using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void):
    pLast_(Path("a.b"), cb_),
    p_(Path("a.b"), cb_)
  {
    while(pLast_.hasNext())
      ++pLast_;
  }

  TestParams::ResultCallback cb_;
  TestParams                 pLast_;
  TestParams                 p_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/ErrorThrower");
} // unnamed namespace


namespace tut
{

// test exception when end is found
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    ErrorThrower::throwOnLast(SYSTEM_SAVE_LOCATION, pLast_);
    fail("throwOnLast() didn't throw on end");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test if non-end does not throw
template<>
template<>
void testObj::test<2>(void)
{
  ErrorThrower::throwOnLast(SYSTEM_SAVE_LOCATION, p_);
}

// test throwing on non-end
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    ErrorThrower::throwOnNotLast(SYSTEM_SAVE_LOCATION, p_);
    fail("throwOnNotLast() didn't throw on non-end path");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test if end does not throw
template<>
template<>
void testObj::test<4>(void)
{
  ErrorThrower::throwOnNotLast(SYSTEM_SAVE_LOCATION, pLast_);
}

// test thwoing on unexpected path token
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    ErrorThrower::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p_);
    fail("throwOnInvalidPath() didn't throw");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test if end does not throw when not reached
template<>
template<>
void testObj::test<6>(void)
{
  ErrorThrower::throwOnEnd(SYSTEM_SAVE_LOCATION, pLast_);
}

// test thwoing on end()
template<>
template<>
void testObj::test<7>(void)
{
  ++pLast_;
  try
  {
    ErrorThrower::throwOnEnd(SYSTEM_SAVE_LOCATION, pLast_);
    fail("throwOnEnd() didn't throw on end");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test throwing on invalid index
template<>
template<>
void testObj::test<8>(void)
{
  try
  {
    ErrorThrower::throwOnInvalidIndex(SYSTEM_SAVE_LOCATION, pLast_);
    fail("throwOnInvalidIndex() didn't throw");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test throwing on invalid name
template<>
template<>
void testObj::test<9>(void)
{
  try
  {
    ErrorThrower::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p_, "invalidname");
    fail("throwOnInvalidName() didn't throw");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

// test no-throwing on valid name
template<>
template<>
void testObj::test<10>(void)
{
  ErrorThrower::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p_, "a");
}

// test no-throwing on index
template<>
template<>
void testObj::test<11>(void)
{
  TestParams p(Path("42.xyz"), cb_);
  ErrorThrower::throwOnInvalidName(SYSTEM_SAVE_LOCATION, p, "thething");
}

} // namespace tut
