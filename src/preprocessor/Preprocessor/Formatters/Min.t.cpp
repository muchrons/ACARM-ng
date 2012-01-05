/*
 * Min.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Min.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Min>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Min");
} // unnamed namespace


namespace tut
{

// test exception on empty list
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    make(0);
  }
  catch(const ExceptionInvalidNumberOfArguments&)
  {
    // this is expected
  }
}

// test if one argument fails as well
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    make(1);
  }
  catch(const ExceptionInvalidNumberOfArguments&)
  {
    // this is expected
  }
}

// test two arguments - strings
template<>
template<>
void testObj::test<3>(void)
{
  BasePtrNN b=make(2);
  values_.at(0)->set("x");
  values_.at(1)->set("aaa");
  ensure_equals("invalid response", b->exec(), "aaa");
}

// test multiple arguments
template<>
template<>
void testObj::test<4>(void)
{
  BasePtrNN b=make(4);
  values_.at(0)->set("ma");
  values_.at(1)->set("miec");
  values_.at(2)->set("ala");
  values_.at(3)->set("kota");
  ensure_equals("invalid response", b->exec(), "ala");
}

// test numbers
template<>
template<>
void testObj::test<5>(void)
{
  BasePtrNN b=make(3);
  values_.at(0)->set("20");
  values_.at(1)->set("1");
  values_.at(2)->set("3");
  ensure_equals("invalid response", b->exec(), "1");
}

// test negative numbers
template<>
template<>
void testObj::test<6>(void)
{
  BasePtrNN b=make(2);
  values_.at(0)->set("-20");
  values_.at(1)->set("1");
  ensure_equals("invalid response", b->exec(), "-20");
}

// test fp-numbers
template<>
template<>
void testObj::test<7>(void)
{
  BasePtrNN b=make(2);
  values_.at(0)->set("2.0");
  values_.at(1)->set("9.5");
  ensure_equals("invalid response", b->exec(), "2");
}

} // namespace tut
