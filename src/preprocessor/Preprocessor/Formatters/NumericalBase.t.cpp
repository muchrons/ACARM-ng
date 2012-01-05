/*
 * NumericalBase.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/NumericalBase.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestImpl: public NumericalBase
{
  explicit TestImpl(ValuePtrNN v):
    NumericalBase( "testimpl", Formatters(1, v) )
  {
  }

  virtual double execConverted(const NumericalArguments &args) const
  {
    tut::ensure("not enought arguments", args.size()>0 );
    return args.at(0);
  }
}; // struct TestImpl


struct TestClass: public TestBase<TestImpl>
{
  TestClass(void):
    ti_(v_)
  {
  }

  TestImpl ti_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/TestImplBase");
} // unnamed namespace


namespace tut
{

// test some numeric value
template<>
template<>
void testObj::test<1>(void)
{
  v_->set("123");
  ensure_equals("invalid value returned", ti_.exec(), "123");
}

// test exception propagation when non-number is spotted
template<>
template<>
void testObj::test<2>(void)
{
  v_->set("abc");
  try
  {
    ti_.exec();     // should throw
    fail("exec() didn't throw on non-numerica value");
  }
  catch(const ExceptionNotANumber&)
  {
    // this is expected
  }
}

// test negative integer
template<>
template<>
void testObj::test<3>(void)
{
  v_->set("-123");
  ensure_equals("invalid value returned", ti_.exec(), "-123");
}

// test positive fp-value
template<>
template<>
void testObj::test<4>(void)
{
  v_->set("1.25");
  ensure_equals("invalid value returned", ti_.exec(), "1.25");
}

// test negative fp-value
template<>
template<>
void testObj::test<5>(void)
{
  v_->set("-1.25");
  ensure_equals("invalid value returned", ti_.exec(), "-1.25");
}

} // namespace tut
