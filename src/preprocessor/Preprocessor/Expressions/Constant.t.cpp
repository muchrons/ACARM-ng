/*
 * Constant.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/Constant.hpp"
#include "Preprocessor/Expressions/TestsCommon.t.hpp"

using namespace Preprocessor::Expressions;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Expressions/Constant");
} // unnamed namespace


namespace tut
{

// test True
template<>
template<>
void testObj::test<1>(void)
{
  True t;
  ensure("invalid value", t.compute( makeAlert() )==true );
}

// test False
template<>
template<>
void testObj::test<2>(void)
{
  False f;
  ensure("invalid value", f.compute( makeAlert() )==false );
}

// test parametrized c-tor
template<>
template<>
void testObj::test<3>(void)
{
  Constant c(false);
  ensure("invalid value", c.compute( makeAlert() )==false );
}

} // namespace tut
