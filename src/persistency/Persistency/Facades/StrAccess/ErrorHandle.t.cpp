/*
 * ErrorHandle.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/ErrorHandle.hpp"
#include "Persistency/Facades/StrAccess/DefaultHandleMap.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency::Facades::StrAccess;
#if 0               // TODO                         

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

factory tf("Persistency/Facades/StrAccess/ErrorHandle");
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
    ErrorHandle::throwIfLast(SYSTEM_SAVE_LOCATION, pLast_);
    fail("throwIfLast() didn't throw on end");
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
  ErrorHandle::throwIfLast(SYSTEM_SAVE_LOCATION, p_);
}

// test throwing on non-end
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    ErrorHandle::throwIfNotLast(SYSTEM_SAVE_LOCATION, p_);
    fail("throwIfNotLast() didn't throw on non-end path");
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
  ErrorHandle::throwIfNotLast(SYSTEM_SAVE_LOCATION, pLast_);
}

// test thwoing on unexpected path token
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    ErrorHandle::throwOnInvalidPath(SYSTEM_SAVE_LOCATION, p_);
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
  ErrorHandle::throwIfEnd(SYSTEM_SAVE_LOCATION, pLast_);
}

// test thwoing on end()
template<>
template<>
void testObj::test<7>(void)
{
  ++pLast_;
  try
  {
    ErrorHandle::throwIfEnd(SYSTEM_SAVE_LOCATION, pLast_);
    fail("throwIfEnd() didn't throw on end");
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
    ErrorHandle::throwOnInvalidIndex(SYSTEM_SAVE_LOCATION, pLast_);
    fail("throwOnInvalidIndex() didn't throw");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

} // namespace tut
#endif
