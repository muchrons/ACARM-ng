/*
 * Mod.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Mod.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Mod>
{
  TestClass(void):
    t_(a_, b_)
  {
  }

  template<typename TEx>
  void testException(const double a, const double b)
  {
    a_->set(a);
    b_->set(b);
    try
    {
      t_.exec();    // should throw
      tut::fail("no exception on error");
    }
    catch(const TEx&)
    {
      // this is expected
    }
  }

  Mod t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Mod");
} // unnamed namespace


namespace tut
{

// test 3%1
template<>
template<>
void testObj::test<1>(void)
{
  a_->set("3");
  b_->set("1");
  ensure_equals("invalid value", t_.exec(), "0");
}

// test 5%2
template<>
template<>
void testObj::test<2>(void)
{
  a_->set("5");
  b_->set("2");
  ensure_equals("invalid value", t_.exec(), "1");
}

// test 8%6
template<>
template<>
void testObj::test<3>(void)
{
  a_->set("8");
  b_->set("6");
  ensure_equals("invalid value", t_.exec(), "2");
}

// test exception negative a
template<>
template<>
void testObj::test<4>(void)
{
  testException<ExceptionUnexpectedNegativeNumber>(-4, 2);
}

// test exception on negative b
template<>
template<>
void testObj::test<5>(void)
{
  testException<ExceptionUnexpectedNegativeNumber>(4, -2);
}

// test exception on floating point a
template<>
template<>
void testObj::test<6>(void)
{
  testException<ExceptionUnexpectedFloatingPoint>(4.2, 2);
}

// test exception on floating point b
template<>
template<>
void testObj::test<7>(void)
{
  testException<ExceptionUnexpectedFloatingPoint>(4, 2.4);
}

// test exception on division by zero
template<>
template<>
void testObj::test<8>(void)
{
  testException<ExceptionDivisionByZero>(8, 0);
}

} // namespace tut
