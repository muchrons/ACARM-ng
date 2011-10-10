/*
 * Not.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/Not.hpp"
#include "Preprocessor/Expressions/TestsCommon.t.hpp"

using namespace Preprocessor::Expressions;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Expressions/Not");
} // unnamed namespace


namespace tut
{

// test negating 'true'
template<>
template<>
void testObj::test<1>(void)
{
  TestExpression *te=new TestExpression(true);
  BasePtrNN       b(te);
  Not             n(b);
  ensure_equals("compute() called in c-tor", te->calls_, 0u);
  ensure("invalid value", n.compute( makeAlert() )==false );
  ensure_equals("compute() not called", te->calls_, 1u);
}

// test negating 'false'
template<>
template<>
void testObj::test<2>(void)
{
  TestExpression *te=new TestExpression(false);
  BasePtrNN       b(te);
  Not             n(b);
  ensure_equals("compute() called in c-tor", te->calls_, 0u);
  ensure("invalid value", n.compute( makeAlert() )==true );
  ensure_equals("compute() not called", te->calls_, 1u);
}

} // namespace tut
