/*
 * Base.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/Base.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestImpl: public Base
{
  TestImpl(void):
    Base("testformatter", Formatters())
  {
  }

  explicit TestImpl(const Formatters &fmts):
    Base("testformatter", fmts)
  {
  }

  std::string execImpl(const Arguments &args) const
  {
    args_=args;
    return ret_;
  }

  mutable Arguments args_;
  string            ret_;
};

struct TestClass
{
  BasePtrNN make(const std::string &ret) const
  {
    TestImpl *ti=new TestImpl;
    BasePtrNN ptr(ti);
    ti->ret_=ret;
    return ptr;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/Base");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  BasePtrNN ptr=make("");
}

// test executing when no args are present
template<>
template<>
void testObj::test<2>(void)
{
  BasePtrNN ptr=make("hello earth creature");
  ensure_equals("invalid exec result", ptr->exec(), "hello earth creature");
}

// test executing with two args
template<>
template<>
void testObj::test<3>(void)
{
  TestImpl::Formatters fmts;
  fmts.push_back( make("A") );
  fmts.push_back( make("B") );
  TestImpl ti(fmts);
  ti.ret_="r";
  ensure_equals("invalid return value", ti.exec(), "r");
  ensure_equals("invalid number of arguments", ti.args_.size(), 2);
  ensure_equals("invalid argument 0", ti.args_.at(0), "A");
  ensure_equals("invalid argument 1", ti.args_.at(1), "B");
}

} // namespace tut
