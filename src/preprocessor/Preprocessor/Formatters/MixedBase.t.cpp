/*
 * MixedBase.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/MixedBase.hpp"
#include "Preprocessor/Formatters/Value.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestImpl: public MixedBase
{
  explicit TestImpl(ValuePtrNN v):
    MixedBase( "testimpl", Formatters(1, v) ),
    nonNumbers_(0)
  {
  }

  virtual void onNonNumeric(const std::string &/*v*/) const
  {
    ++nonNumbers_;
  }
  virtual double execConverted(const NumericalArguments &/*args*/) const
  {
    return 42.5;
  }
  virtual std::string execConverted(const Arguments &/*args*/) const
  {
    return "true, true...";
  }


  mutable unsigned int nonNumbers_;
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

factory tf("Preprocessor/Formatters/MixedBase");
} // unnamed namespace


namespace tut
{

// test some numeric value
template<>
template<>
void testObj::test<1>(void)
{
  v_->set("123");
  ensure_equals("invalid value returned", ti_.exec(), "42.5");
  ensure_equals("invalid number of non-numbers", ti_.nonNumbers_, 0);
}

// test non-numeric value
template<>
template<>
void testObj::test<2>(void)
{
  v_->set("string value");
  ensure_equals("invalid value returned", ti_.exec(), "true, true...");
  ensure_equals("invalid number of non-numbers", ti_.nonNumbers_, 1);
}


namespace
{
struct TestError: public TestImpl
{
  explicit TestError(ValuePtrNN v):
    TestImpl(v)
  {
  }

  virtual void onNonNumeric(const std::string &v) const
  {
    ensure_equals("invalid error paramter", v, "abc");
    throw std::runtime_error("got something");
  }
};
} // unnamed namespace

// test exception propagation when non-number is spotted
template<>
template<>
void testObj::test<3>(void)
{
  TestError te(v_);
  v_->set("abc");
  try
  {
    te.exec();  // should throw
    fail("exec() didn't throw when error shouldbe fatal (i.e. exceptions are not propagated)");
  }
  catch(const std::runtime_error&)
  {
    // this is expected
  }
}

} // namespace tut
