/*
 * Add.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Add.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Add>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Add");
} // unnamed namespace


namespace tut
{

// test 2+0+4
template<>
template<>
void testObj::test<1>(void)
{
  BasePtrNN p=make(3);
  values_.at(0)->set(2);
  values_.at(1)->set(0);
  values_.at(2)->set(4);
  ensure_equals("invalid result", p->exec(), "6");
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

// test addition of two elements
template<>
template<>
void testObj::test<4>(void)
{
  BasePtrNN p=make(2);
  values_.at(0)->set(2);
  values_.at(1)->set(-3);
  ensure_equals("invalid result", p->exec(), "-1");
}

} // namespace tut
