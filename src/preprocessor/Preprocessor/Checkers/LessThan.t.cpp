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

// test positive comparison
template<>
template<>
void testObj::test<2>(void)
{
  const LessThan ce("abc");
  ensure("comparison failed", ce.check("a") );
}

// test negative comparison
template<>
template<>
void testObj::test<3>(void)
{
  const LessThan ce("abc");
  ensure("comparison didn't failed", ce.check("doom")==false );
}

// test numerical comarison - positive case
template<>
template<>
void testObj::test<4>(void)
{
  const LessThan ce("42");
  ensure("comparison failed", ce.check("0041") );
}

// test numerical comarison - negative case
template<>
template<>
void testObj::test<5>(void)
{
  const LessThan ce("42");
  ensure("comparison didn't failed", ce.check("0043")==false );
}

} // namespace tut
