/*
 * CheckEquals.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/CheckEquals.hpp"

using namespace Preprocessor;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/CheckEquals");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  CheckEquals ce("abc");
}

// test positive comparison
template<>
template<>
void testObj::test<2>(void)
{
  const CheckEquals ce("abc");
  ensure("comparison failed", ce.check("abc") );
}

// test negative comparison
template<>
template<>
void testObj::test<3>(void)
{
  const CheckEquals ce("abc");
  ensure("comparison didn't failed", ce.check("abcXYZ")==false );
}

} // namespace tut
