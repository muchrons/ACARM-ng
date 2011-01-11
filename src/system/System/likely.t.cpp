/*
 * likely.t.cpp
 *
 */
#include <tut.h>

#include "System/likely.hpp"

namespace System
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/likely");
}

using namespace System;

namespace tut
{

// true condition for LIKELY()
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals( LIKELY(true), true);
}

// true condition for UNLIKELY()
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals( UNLIKELY(true), true);
}

// false for LIKELY()
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals( LIKELY(false), false);
}

// false for UNLIKELY()
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals( UNLIKELY(false), false);
}

// test LIKELY for integer
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals( LIKELY(42), true);
}

// test UNLIKELY for integer
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals( UNLIKELY(42), true);
}

namespace
{
bool makeTest(int i1, int i2)
{
  return i1==i2;
} // makeTest()
} // unnamed namespace

// test LIKELY for funciton call with comma
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals( UNLIKELY( makeTest(21, 32) ), false);
}

// test UNLIKELY for funciton call with comma
template<>
template<>
void testObj::test<8>(void)
{
  ensure_equals( LIKELY( makeTest(21, 32) ), false);
}

// test LIKELY for comparison
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals( UNLIKELY(10==11), false);
}

// test UNLIKELY for comparison
template<>
template<>
void testObj::test<10>(void)
{
  ensure_equals( LIKELY(20==11), false);
}

} // namespace tut
