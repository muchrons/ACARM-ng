/*
 * FormatterBuilder.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Singleton.hpp"
#include "Preprocessor/Expressions/FormatterBuilder.hpp"

using ConfigIO::Preprocessor::FormatterConfig;
using Preprocessor::Formatters::BasePtrNN;
using Preprocessor::Formatters::ValuePtrNN;
using namespace Preprocessor::Expressions;


namespace
{
struct TestClass
{
  TestClass(void):
    value_(new Preprocessor::Formatters::Value),
    fb_(value_)
  {
  }

  FormatterConfig makeFormatterConfig(const std::string &filename) const
  {
    const std::string path="testdata/formatters_cfg/" + filename;
    ConfigIO::Singleton::get()->rereadConfig( path.c_str() );
    const ConfigIO::Preprocessor::Section &s=ConfigIO::Singleton::get()->preprocessorConfig().getSections().at(0);
    return s.getExpression().getRules().at(0).getFormatter();
  }

  template<typename TEx>
  void testException(const char *xmlFile, const char *failMessage) const
  {
    try
    {
      fb_.build( makeFormatterConfig(xmlFile) );
      tut::fail(failMessage);
    }
    catch(const TEx&)
    {
      // this is expected
    }
  }

  void testArgsExcept(const char *xmlFile, const char *failMessage) const
  {
    testException<FormatterBuilder::ExceptionInvalidNumberOfArguments>(xmlFile, failMessage);
  }


  ValuePtrNN             value_;
  const FormatterBuilder fb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Expressions/FormatterBuilder");
} // unnamed namespace


namespace tut
{

// test default formatter's creation
template<>
template<>
void testObj::test<1>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("no_formatter.xml") );
  value_->set("narf");
  ensure_equals("invalid returned value", b->exec(), "narf");
}

// test abs() formatter
template<>
template<>
void testObj::test<2>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("abs.xml") );
  ensure_equals("invalid returned value", b->exec(), "666");
}

// test ceil() formatter
template<>
template<>
void testObj::test<3>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("ceil.xml") );
  ensure_equals("invalid returned value", b->exec(), "4");
}

// test more complex formatter
template<>
template<>
void testObj::test<4>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("complex.xml") );
  ensure_equals("invalid returned value", b->exec(), "7");
}

// test div() formatter
template<>
template<>
void testObj::test<5>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("div.xml") );
  ensure_equals("invalid returned value", b->exec(), "8");
}

// test floor() formatter
template<>
template<>
void testObj::test<6>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("floor.xml") );
  ensure_equals("invalid returned value", b->exec(), "2");
}

// test max() formatter
template<>
template<>
void testObj::test<7>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("max.xml") );
  ensure_equals("invalid returned value", b->exec(), "11");
}

// test min() formatter
template<>
template<>
void testObj::test<8>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("min.xml") );
  ensure_equals("invalid returned value", b->exec(), "-1");
}

// test mul() formatter
template<>
template<>
void testObj::test<9>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("mul.xml") );
  ensure_equals("invalid returned value", b->exec(), "12");
}

// test round() formatter
template<>
template<>
void testObj::test<10>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("round.xml") );
  ensure_equals("invalid returned value", b->exec(), "7");
}

// test sig() formatter
template<>
template<>
void testObj::test<11>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("sig.xml") );
  ensure_equals("invalid returned value", b->exec(), "-1");
}

// test sub() formatter
template<>
template<>
void testObj::test<12>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("sub.xml") );
  ensure_equals("invalid returned value", b->exec(), "42");
}

// test tsProc() formatter
template<>
template<>
void testObj::test<13>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("tsProc.xml") );
  ensure_equals("invalid returned value", b->exec(), "11:06");
}

// test value() formatter
template<>
template<>
void testObj::test<14>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("value.xml") );
  value_->set("evil empire");
  ensure_equals("invalid returned value", b->exec(), "evil empire");
}

// test add() formatter
template<>
template<>
void testObj::test<15>(void)
{
  BasePtrNN b=fb_.build( makeFormatterConfig("add.xml") );
  ensure_equals("invalid returned value", b->exec(), "42");
}

// test building invalid function
template<>
template<>
void testObj::test<16>(void)
{
  testException<FormatterBuilder::ExceptionUnknownFunction>("invalid_function.xml", "no exception uppon invalid function");
}

// test abs() with too many arguments
template<>
template<>
void testObj::test<17>(void)
{
  testArgsExcept("abs_too_many.xml", "no exception on too many arguments");
}

// test abs() with too little arguments
template<>
template<>
void testObj::test<18>(void)
{
  testArgsExcept("abs_too_little.xml", "no exception on too little arguments");
}

// test add() with too little arguments
template<>
template<>
void testObj::test<19>(void)
{
  testArgsExcept("add_too_little.xml", "no exception on too little arguments");
}

// test ceil() with too many arguments
template<>
template<>
void testObj::test<20>(void)
{
  testArgsExcept("ceil_too_many.xml", "no exception on too many arguments");
}

// test ceil() with too little arguments
template<>
template<>
void testObj::test<21>(void)
{
  testArgsExcept("ceil_too_little.xml", "no exception on too little arguments");
}

// test div() with too little arguments
template<>
template<>
void testObj::test<22>(void)
{
  testArgsExcept("div_too_little.xml", "no exception on too little arguments");
}

// test div() with too many arguments
template<>
template<>
void testObj::test<23>(void)
{
  testArgsExcept("div_too_many.xml", "no exception on too many arguments");
}

// test floor() with too little arguments
template<>
template<>
void testObj::test<24>(void)
{
  testArgsExcept("floor_too_little.xml", "no exception on too little arguments");
}

// test floor() with too many arguments
template<>
template<>
void testObj::test<25>(void)
{
  testArgsExcept("floor_too_many.xml", "no exception on too many arguments");
}

// test max() with too little arguments
template<>
template<>
void testObj::test<26>(void)
{
  testArgsExcept("max_too_little.xml", "no exception on too little arguments");
}

// test min() with too little arguments
template<>
template<>
void testObj::test<27>(void)
{
  testArgsExcept("min_too_little.xml", "no exception on too little arguments");
}

// test mul() with too little arguments
template<>
template<>
void testObj::test<28>(void)
{
  testArgsExcept("mul_too_little.xml", "no exception on too little arguments");
}

// test round() with too little arguments
template<>
template<>
void testObj::test<29>(void)
{
  testArgsExcept("round_too_little.xml", "no exception on too little arguments");
}

// test round() with too many arguments
template<>
template<>
void testObj::test<30>(void)
{
  testArgsExcept("round_too_many.xml", "no exception on too many arguments");
}

// test sig() with too little arguments
template<>
template<>
void testObj::test<31>(void)
{
  testArgsExcept("sig_too_little.xml", "no exception on too little arguments");
}

// test sig() with too many arguments
template<>
template<>
void testObj::test<32>(void)
{
  testArgsExcept("sig_too_many.xml", "no exception on too many arguments");
}

// test sub() with too little arguemnts
template<>
template<>
void testObj::test<33>(void)
{
  testArgsExcept("sub_too_little.xml", "no exception on too little arguments");
}

// test sub() with too many arguments
template<>
template<>
void testObj::test<34>(void)
{
  testArgsExcept("sub_too_many.xml", "no exception on too many arguments");
}

// test tsProc() with too many arguments
template<>
template<>
void testObj::test<35>(void)
{
  testArgsExcept("tsProc_too_many.xml", "no exception on too many arguments");
}

// test tsProc() with too little arguments
template<>
template<>
void testObj::test<36>(void)
{
  testArgsExcept("tsProc_too_little.xml", "no exception on too little arguments");
}

// test tsProc() with non-string format argument
template<>
template<>
void testObj::test<37>(void)
{
  testException<FormatterBuilder::ExceptionInvalidArgument>("tsProc_func_format.xml", "no exception on invalid format");
}

} // namespace tut
