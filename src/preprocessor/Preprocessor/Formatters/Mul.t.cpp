/*
 * Mul.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Mul.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Mul>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Mul");
} // unnamed namespace


namespace tut
{

// test 2*0*4
template<>
template<>
void testObj::test<1>(void)
{
  BasePtrNN p=make(3);
  values_.at(0)->set(2);
  values_.at(1)->set(0);
  values_.at(2)->set(4);
  ensure_equals("invalid result", p->exec(), "0");
}

// test exception when building with no arguments
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    make(0);
    fail("building with no args didn't throw");
  }
  catch(const ExceptionInvalidNumberOfArguments&)
  {
    // this is expected
  }
}

// test exception when building with one argument
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    make(1);
    fail("building with 1 arg didn't throw");
  }
  catch(const ExceptionInvalidNumberOfArguments&)
  {
    // this is expected
  }
}

// test multiplily of two elements
template<>
template<>
void testObj::test<4>(void)
{
  BasePtrNN p=make(2);
  values_.at(0)->set(2);
  values_.at(1)->set(-3);
  ensure_equals("invalid result", p->exec(), "-6");
}

// test multiplily of more, non-zero elements
template<>
template<>
void testObj::test<5>(void)
{
  BasePtrNN p=make(4);
  values_.at(0)->set(2);
  values_.at(1)->set(-3);
  values_.at(2)->set(-4);
  values_.at(3)->set(5);
  ensure_equals("invalid result", p->exec(), "120");
}
//TODO: tests for fp numbers should be added
} // namespace tut
