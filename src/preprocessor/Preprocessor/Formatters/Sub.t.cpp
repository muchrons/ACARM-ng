/*
 * Sub.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Sub.hpp"
#include "Preprocessor/Formatters/Value.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass
{
  TestClass(void):
    a_(new Value),
    b_(new Value),
    t_(a_, b_)
  {
  }

  ValuePtrNN a_;
  ValuePtrNN b_;
  Sub        t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Sub");
} // unnamed namespace


namespace tut
{

// test 3-1
template<>
template<>
void testObj::test<1>(void)
{
  a_->set("3");
  b_->set("1");
  ensure_equals("invalid value", t_.exec(), "2");
}

// test -3-(-2)
template<>
template<>
void testObj::test<2>(void)
{
  a_->set("-3");
  b_->set("-2");
  ensure_equals("invalid value", t_.exec(), "-1");
}

// test 3-(-1)
template<>
template<>
void testObj::test<3>(void)
{
  a_->set("3");
  b_->set("-1");
  ensure_equals("invalid value", t_.exec(), "4");
}

// test 0-0
template<>
template<>
void testObj::test<4>(void)
{
  a_->set("0");
  b_->set("0");
  ensure_equals("invalid value", t_.exec(), "0");
}

// test 2-2
template<>
template<>
void testObj::test<5>(void)
{
  a_->set("2");
  b_->set("2");
  ensure_equals("invalid value", t_.exec(), "0");
}

// test 66-0
template<>
template<>
void testObj::test<6>(void)
{
  a_->set("666");
  b_->set("0");
  ensure_equals("invalid value", t_.exec(), "666");
}

} // namespace tut
