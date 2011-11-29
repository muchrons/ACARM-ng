/*
 * NumberWrapper.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Checkers/NumberWrapper.hpp"

using namespace Preprocessor::Checkers;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Checkers/NumberWrapper");
} // unnamed namespace


namespace tut
{

// test setting non-number
template<>
template<>
void testObj::test<1>(void)
{
  const NumberWrapper nc("123a");
  ensure("non-number parsed", nc.isNumber()==false );
}

// test empty string
template<>
template<>
void testObj::test<2>(void)
{
  const NumberWrapper nc("");
  ensure("non-number parsed", nc.isNumber()==false );
}

// test positive number
template<>
template<>
void testObj::test<3>(void)
{
  const NumberWrapper nc("42");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), 42);
}

// test negative number
template<>
template<>
void testObj::test<4>(void)
{
  const NumberWrapper nc("-42");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), -42);
}

// test positive floating point number
template<>
template<>
void testObj::test<5>(void)
{
  const NumberWrapper nc("4.25");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), 4.25);
}

// test negative floating point number
template<>
template<>
void testObj::test<6>(void)
{
  const NumberWrapper nc("-4.25");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), -4.25);
}

// test minus in wrong place
template<>
template<>
void testObj::test<7>(void)
{
  const NumberWrapper nc("42-5");
  ensure("non-number parsed", nc.isNumber()==false );
}

// test multiple dots
template<>
template<>
void testObj::test<8>(void)
{
  const NumberWrapper nc("4.2.5");
  ensure("non-number parsed", nc.isNumber()==false );
}

// test leading '+' sign
template<>
template<>
void testObj::test<9>(void)
{
  const NumberWrapper nc("+42");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), 42);
}

// test leading dot
template<>
template<>
void testObj::test<10>(void)
{
  const NumberWrapper nc(".5");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), 0.5);
}

// test trailing dot
template<>
template<>
void testObj::test<11>(void)
{
  const NumberWrapper nc("1.");
  ensure("number not parsed", nc.isNumber() );
  ensure_equals("invalid value", nc.get(), 1);
}

} // namespace tut
