/*
 * LessThan.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Checkers/LessThan.hpp"

using namespace Preprocessor::Checkers;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Checkers/LessThan");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  LessThan ce("abc");
}

// test positive comparison for a non-number
template<>
template<>
void testObj::test<2>(void)
{
  const LessThan ce("abc");
  ensure("comparison failed", ce.check("Z") );
}

// test negative comparison for a non-number
template<>
template<>
void testObj::test<3>(void)
{
  const LessThan ce("abc");
  ensure("comparison didn't failed", ce.check("ax")==false );
}

// test positive comparison for a number
template<>
template<>
void testObj::test<4>(void)
{
  const LessThan ce("42");
  ensure("comparison failed", ce.check("5") );
}

// test negative comparison for a number
template<>
template<>
void testObj::test<5>(void)
{
  const LessThan ce("42");
  ensure("comparison didn't failed", ce.check("50")==false );
}

// test comparison when c-tor argument is not a number, but method paramter is a number
template<>
template<>
void testObj::test<6>(void)
{
  const LessThan ce("test");
  ensure("comparison didn't failed", ce.check("10") );
}

// test comparison when c-tor argument is not a number, but method parameter is a number
template<>
template<>
void testObj::test<7>(void)
{
  const LessThan ce("50");
  ensure("comparison failed", ce.check("test")==false );
}

// test positive comparison for a non-number, starting with numbers
template<>
template<>
void testObj::test<8>(void)
{
  const LessThan ce("2a");
  ensure("comparison failed", ce.check("21a") );
}

// test negative comparison for a non-number, starting with nu,bers
template<>
template<>
void testObj::test<9>(void)
{
  const LessThan ce("22a");
  ensure("comparison didn't failed", ce.check("3a")==false );
}

// test failure for identical strings
template<>
template<>
void testObj::test<10>(void)
{
  const LessThan ce("narf");
  ensure("comparison didn't failed", ce.check("narf")==false );
}

// test failure for identical numbers
template<>
template<>
void testObj::test<11>(void)
{
  const LessThan ce("42");
  ensure("comparison didn't failed", ce.check("42")==false );
}

} // namespace tut
