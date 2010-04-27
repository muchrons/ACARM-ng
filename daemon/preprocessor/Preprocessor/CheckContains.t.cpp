/*
 * CheckContains.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/CheckContains.hpp"

using namespace Preprocessor;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/CheckContains");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  CheckContains ce("abc");
}

// test positive comparison
template<>
template<>
void testObj::test<2>(void)
{
  const CheckContains ce("cat");
  ensure("comparison failed", ce.check("alice has a cat") );
}

// test negative comparison
template<>
template<>
void testObj::test<3>(void)
{
  const CheckContains ce("dog");
  ensure("comparison didn't failed", ce.check("alice has a cat")==false );
}

} // namespace tut
