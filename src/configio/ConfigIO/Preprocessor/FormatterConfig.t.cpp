/*
 * FormatterConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Preprocessor/FormatterConfig.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{

struct TestClass
{
  TestClass(void)
  {
    value_.type_ =detail::FormatterConfigData::VALUE;

    param1_.type_=detail::FormatterConfigData::ARGUMENT;
    param1_.str_ ="p1";

    param2_.type_=detail::FormatterConfigData::ARGUMENT;
    param2_.str_ ="p2";

    func1_.type_ =detail::FormatterConfigData::FUNCTION;
    func1_.str_  ="f1";
    func1_.args_.push_back(param1_);

    func2_.type_ =detail::FormatterConfigData::FUNCTION;
    func2_.str_  ="f2";
    func2_.args_.push_back(func1_);
    func2_.args_.push_back(param2_);
  }

  detail::FormatterConfigData value_;
  detail::FormatterConfigData param1_;
  detail::FormatterConfigData param2_;
  detail::FormatterConfigData func1_;
  detail::FormatterConfigData func2_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/FormatterConfig");
} // unnamed namespace


namespace tut
{

// test value
template<>
template<>
void testObj::test<1>(void)
{
  const FormatterConfig::Wrapper w(value_);
  ensure("invalid value type", w.isValue() );
}

// test argument
template<>
template<>
void testObj::test<2>(void)
{
  const FormatterConfig::Wrapper w(param1_);
  ensure("invalid argument type", w.isArgument() );
  ensure_equals("invalid argument's value", w.argument(), "p1");
}

// test function with one argument
template<>
template<>
void testObj::test<3>(void)
{
  const FormatterConfig::Wrapper w(func1_);
  ensure("invalid function type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "f1");
  ensure_equals("invalid parameter's count", w.argCount(), 1);
  ensure("invalid parameter 0 type", w.param(0).isArgument() );
  ensure_equals("invalid parameter 0 value", w.param(0).argument(), "p1");
}

// test fucntion with a funciton as an argument
template<>
template<>
void testObj::test<4>(void)
{
  const FormatterConfig::Wrapper w(func2_);
  ensure("invalid function type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "f2");
  ensure_equals("invalid parameter's count", w.argCount(), 2);
  // a1
  ensure("invalid parameter 0 type", w.param(0).isFunction() );
  ensure_equals("invalid parameter 0 name", w.param(0).name(), "f1");
  // a2
  ensure("invalid parameter 1 type", w.param(1).isArgument() );
  ensure_equals("invalid parameter 1 value", w.param(1).argument(), "p2");
}

// test if Wrapper is copyable
template<>
template<>
void testObj::test<5>(void)
{
  const FormatterConfig::Wrapper w1(func2_);
  const FormatterConfig::Wrapper w2(w1);
}

// test getting wrapper via formatter config
template<>
template<>
void testObj::test<6>(void)
{
  const FormatterConfig          fc(func1_);
  const FormatterConfig::Wrapper w( fc.get() );
}

} // namespace tut
