/*
 * Div.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Div.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Div>
{
  TestClass(void):
    t_(a_, b_)
  {
  }

  Div t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Div");
} // unnamed namespace


namespace tut
{

// test 1/2
template<>
template<>
void testObj::test<1>(void)
{
  a_->set("1");
  b_->set("2");
  ensure_equals("invalid value", t_.exec(), "0.5");
}

// test -8/-2
template<>
template<>
void testObj::test<2>(void)
{
  a_->set("-8");
  b_->set("-2");
  ensure_equals("invalid value", t_.exec(), "4");
}

// test -8/2
template<>
template<>
void testObj::test<3>(void)
{
  a_->set("-8");
  b_->set("2");
  ensure_equals("invalid value", t_.exec(), "-4");
}

// test 2.5/10
template<>
template<>
void testObj::test<4>(void)
{
  a_->set("2.5");
  b_->set("10");
  ensure_equals("invalid value", t_.exec(), "0.25");
}

// test 16/1
template<>
template<>
void testObj::test<5>(void)
{
  a_->set("16");
  b_->set("1");
  ensure_equals("invalid value", t_.exec(), "16");
}

// test 16/0
template<>
template<>
void testObj::test<6>(void)
{
  a_->set("16");
  b_->set("0");
  try
  {
    t_.exec();  // should throw
    fail("dicision by zero didn't throw");
  }
  catch(const ExceptionDivisionByZero&)
  {
    // this is expected
  }
}

} // namespace tut
