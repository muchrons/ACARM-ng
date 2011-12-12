/*
 * Abs.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Abs.hpp"
#include "Preprocessor/Formatters/Value.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass
{
  TestClass(void):
    v_(new Value),
    t_(v_)
  {
  }

  ValuePtrNN v_;
  Abs        t_;
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

// test abs(negative/fp)
template<>
template<>
void testObj::test<4>(void)
{
  v_->set("-42.75");
  ensure_equals("invalid value", t_.exec(), "42.75");
}

} // namespace tut
