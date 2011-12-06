/*
 * GreaterThan.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Checkers/GreaterThan.hpp"

using namespace Preprocessor::Checkers;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Checkers/GreaterThan");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  GreaterThan ce("abc");
}

// test positive comparison
template<>
template<>
void testObj::test<2>(void)
{
  const GreaterThan ce("abc");
  ensure("comparison failed", ce.check("doom") );
}

// test negative comparison
template<>
template<>
void testObj::test<3>(void)
{
  const GreaterThan ce("abc");
  ensure("comparison didn't failed", ce.check("a")==false );
}

// test numerical comparison - positive case
template<>
template<>
void testObj::test<4>(void)
{
  const GreaterThan ce("42");
  ensure("comparison failed", ce.check("0043") );
}

// test numerical comparison - negative case
template<>
template<>
void testObj::test<5>(void)
{
  const GreaterThan ce("42");
  ensure("comparison didn't failed", ce.check("0041")==false );
}

// test if equality fails
template<>
template<>
void testObj::test<6>(void)
{
  const GreaterThan ce("42");
  ensure("comparison didn't failed", ce.check("0042")==false );
}

// TODO: add floating point numbers comparison tests if it makes sense
} // namespace tut
