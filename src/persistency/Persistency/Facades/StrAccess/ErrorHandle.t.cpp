/*
 * ErrorHandle.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/ErrorHandle.hpp"
#include "Persistency/Facades/StrAccess/TestCallback.t.hpp"

using namespace Persistency::Facades::StrAccess;

namespace
{

struct TestClass
{
  TestClass(void):
    pEnd_(Path("a.b"), cb_),
    p_(Path("a.b"), cb_)
  {
    while(!pEnd_.isEnd())
      ++pEnd_;
  }

  TestCallback cb_;
  Params       pEnd_;
  Params       p_;
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
    ErrorHandle::throwIfEnd(pEnd_);
    fail("throwIfEnd() didn't throw on end");
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
  ErrorHandle::throwIfEnd(p_);
}

// test throwing on non-end
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    ErrorHandle::throwIfNotEnd(p_);
    fail("throwIfNotEnd() didn't throw on non-end path");
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
  ErrorHandle::throwIfNotEnd(pEnd_);
}

// test thwoing on unexpected path token
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    ErrorHandle::throwOnInvalidPath(p_);
    fail("throwOnInvalidPath() didn't throw");
  }
  catch(const ExceptionInvalidPath &)
  {
    // this is expected
  }
}

} // namespace tut
