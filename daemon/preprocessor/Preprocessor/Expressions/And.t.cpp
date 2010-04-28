/*
 * And.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Expressions/And.hpp"
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

factory tf("Preprocessor/Expressions/And");
} // unnamed namespace


namespace tut
{

// test positive case
template<>
template<>
void testObj::test<1>(void)
{
  TestExpression *te1=new TestExpression(true);
  BasePtrNN       b1(te1);
  TestExpression *te2=new TestExpression(true);
  BasePtrNN       b2(te2);

  And::Expressions e;
  e.push_back(b1);
  e.push_back(b2);
  And a(e);

  ensure_equals("compute()/1 called in c-tor", te1->calls_, 0u);
  ensure_equals("compute()/2 called in c-tor", te1->calls_, 0u);
  ensure("invalid value", a.compute( *TestHelpers::Persistency::makeNewAlert() )==true );
  ensure_equals("compute()/1 not called", te1->calls_, 1u);
  ensure_equals("compute()/2 not called", te2->calls_, 1u);
}

// test negative result
template<>
template<>
void testObj::test<2>(void)
{
  TestExpression *te1=new TestExpression(true);
  BasePtrNN       b1(te1);
  TestExpression *te2=new TestExpression(false);
  BasePtrNN       b2(te2);

  And::Expressions e;
  e.push_back(b1);
  e.push_back(b2);
  And a(e);

  ensure_equals("compute()/1 called in c-tor", te1->calls_, 0u);
  ensure_equals("compute()/2 called in c-tor", te1->calls_, 0u);
  ensure("invalid value", a.compute( *TestHelpers::Persistency::makeNewAlert() )==false );
  ensure_equals("compute()/1 not called", te1->calls_, 1u);
  ensure_equals("compute()/2 not called", te2->calls_, 1u);
}

// test throw on not enought elements
template<>
template<>
void testObj::test<3>(void)
{
  TestExpression   *te=new TestExpression(true);
  BasePtrNN         b(te);
  And::Expressions  e;
  e.push_back(b);
  try
  {
    And a(e);   // should throw
    tut::fail("c-tor didn't thorow on not enought elements");
  }
  catch(const Preprocessor::ExceptionInvalidExpression&)
  {
    // this is expected
  }
}

} // namespace tut
