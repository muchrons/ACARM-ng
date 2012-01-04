/*
 * Sig.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Sig.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<Sig>
{
  TestClass(void):
    t_(v_)
  {
  }

  Sig t_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Sig");
} // unnamed namespace


namespace tut
{

// test sig(0)
template<>
template<>
void testObj::test<1>(void)
{
  v_->set("0");
  ensure_equals("invalid value", t_.exec(), "0");
}

// test sig(positive)
template<>
template<>
void testObj::test<2>(void)
{
  v_->set("42");
  ensure_equals("invalid value", t_.exec(), "1");
}

// test sig(negative)
template<>
template<>
void testObj::test<3>(void)
{
  v_->set("-42");
  ensure_equals("invalid value", t_.exec(), "-1");
}

// test sig(positive-fp)
template<>
template<>
void testObj::test<4>(void)
{
  v_->set("4.25");
  ensure_equals("invalid value", t_.exec(), "1");
}

// test sig(negative-fp)
template<>
template<>
void testObj::test<5>(void)
{
  v_->set("-4.25");
  ensure_equals("invalid value", t_.exec(), "-1");
}

// test non-number
template<>
template<>
void testObj::test<6>(void)
{
  v_->set("narf");
  try
  {
    t_.exec();  // should throw
    fail("no exception on not-a-number error");
  }
  catch(const ExceptionNotANumber &)
  {
    // this is expected
  }
}

} // namespace tut
