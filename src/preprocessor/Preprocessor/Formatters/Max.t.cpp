/*
 * Max.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Max.hpp"
#include "Preprocessor/Formatters/Value.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass
{
  BasePtrNN make(const size_t args)
  {
    values_.clear();
    Base::Formatters f;
    for(size_t i=0; i<args; ++i)
    {
      ValuePtrNN v(new Value);
      values_.push_back(v);
      f.push_back(v);
    }
    return BasePtrNN(new Max(f));
  }

  std::vector<ValuePtrNN> values_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Max");
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
  values_.at(0)->set("aaa");
  values_.at(1)->set("x");
  ensure_equals("invalid response", b->exec(), "x");
}

// test multiple arguments
template<>
template<>
void testObj::test<4>(void)
{
  BasePtrNN b=make(4);
  values_.at(0)->set("ala");
  values_.at(1)->set("ma");
  values_.at(2)->set("miec");
  values_.at(3)->set("kota");
  ensure_equals("invalid response", b->exec(), "miec");
}

// test numbers
template<>
template<>
void testObj::test<5>(void)
{
  BasePtrNN b=make(3);
  values_.at(0)->set("1");
  values_.at(1)->set("20");
  values_.at(2)->set("3");
  ensure_equals("invalid response", b->exec(), "20");
}

} // namespace tut
