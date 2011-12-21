/*
 * Floor.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Floor.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Floor>
{
  TestClass(void):
    t_(v_)
  {
  }

  Floor t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Floor");
} // unnamed namespace


namespace tut
{

// test floor(0)
template<>
template<>
void testObj::test<1>(void)
{
  v_->set("0");
  ensure_equals("invalid value", t_.exec(), "0");
}

// test floor(positive)
template<>
template<>
void testObj::test<2>(void)
{
  v_->set("4.2");
  ensure_equals("invalid value", t_.exec(), "4");
}

// test floor(negative)
template<>
template<>
void testObj::test<3>(void)
{
  v_->set("-4.9");
  ensure_equals("invalid value", t_.exec(), "-5");
}

// test floor(positive)
template<>
template<>
void testObj::test<4>(void)
{
  v_->set("4.9");
  ensure_equals("invalid value", t_.exec(), "4");
}

// test floor(negative)
template<>
template<>
void testObj::test<5>(void)
{
  v_->set("-4.2");
  ensure_equals("invalid value", t_.exec(), "-5");
}

} // namespace tut
