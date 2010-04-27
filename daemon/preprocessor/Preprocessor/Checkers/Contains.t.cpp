/*
 * Contains.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Checkers/Contains.hpp"

using namespace Preprocessor::Checkers;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Checkers/Contains");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Contains ce("abc");
}

// test positive comparison
template<>
template<>
void testObj::test<2>(void)
{
  const Contains ce("cat");
  ensure("comparison failed", ce.check("alice has a cat") );
}

// test negative comparison
template<>
template<>
void testObj::test<3>(void)
{
  const Contains ce("dog");
  ensure("comparison didn't failed", ce.check("alice has a cat")==false );
}

} // namespace tut
