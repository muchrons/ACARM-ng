/*
 * Abs.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Abs.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Abs>
{
  TestClass(void):
    t_(v_)
  {
  }

  Abs t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Abs");
} // unnamed namespace


namespace tut
{

// test abs(0)
template<>
template<>
void testObj::test<1>(void)
{
  v_->set("0");
  ensure_equals("invalid value", t_.exec(), "0");
}

// test abs(positive)
template<>
template<>
void testObj::test<2>(void)
{
  v_->set("42");
  ensure_equals("invalid value", t_.exec(), "42");
}

// test abs(negative)
template<>
template<>
void testObj::test<3>(void)
{
  v_->set("-42");
  ensure_equals("invalid value", t_.exec(), "42");
}

// test abs(negative-fp)
template<>
template<>
void testObj::test<4>(void)
{
  v_->set("-42.75");
  ensure_equals("invalid value", t_.exec(), "42.75");
}

// test abs(positive-fp)
template<>
template<>
void testObj::test<5>(void)
{
  v_->set("42.75");
  ensure_equals("invalid value", t_.exec(), "42.75");
}

// test abs(nan)
template<>
template<>
void testObj::test<6>(void)
{
  v_->set("oops");
  try
  {
    t_.exec();  // should throw
    fail("no exception on non-number for abs()");
  }
  catch(const ExceptionNotANumber&)
  {
    // this is expected
  }
}

} // namespace tut
