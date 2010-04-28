/*
 * Not.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/Not.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Preprocessor::Expressions;

namespace
{

struct TestExpression: public Preprocessor::Expressions::Base
{
  explicit TestExpression(bool ret):
    ret_(ret),
    calls_(0)
  {
  }

  virtual bool compute(const Persistency::Alert &/*alert*/) const
  {
    ++calls_;
    return ret_;
  }

  bool         ret_;
  mutable int  calls_;
};

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
  ensure("invalid negated", n.compute( *TestHelpers::Persistency::makeNewAlert() )==false );
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
  ensure("invalid negated", n.compute( *TestHelpers::Persistency::makeNewAlert() )==true );
  ensure_equals("compute() not called", te->calls_, 1u);
}

} // namespace tut
