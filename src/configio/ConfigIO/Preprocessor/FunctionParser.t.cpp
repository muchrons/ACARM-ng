/*
 * FunctionParser.t.cpp
 *
 */
#include <tut.h>
#include <boost/lexical_cast.hpp>

#include "ConfigIO/Preprocessor/FunctionParser.hpp"

using namespace std;
using namespace ConfigIO::Preprocessor;

namespace
{

struct TestClass
{
  void testThrow(const string &str, const char *message) const
  {
    try
    {
      FunctionParser fp(str);   // should throw
      tut::fail(message);
    }
    catch(const ConfigIO::ExceptionParseError &)
    {
      // this is expected
    }
  }

  void testNumber(const string &num) const
  {
    const string trm=trim(num);
    const double tmp=boost::lexical_cast<double>(trm);
    const string exp=boost::lexical_cast<string>(tmp);
    testNumber(num, exp);
  }

  void testNumber(const string &num, const string &exp) const
  {
    const FunctionParser           fp("fun(" + num + ")");
    const FormatterConfig::Wrapper w=fp.getConfig().get();
    tut::ensure("invalid type", w.isFunction() );
    tut::ensure_equals("invalid function name", w.name(), "fun");
    tut::ensure_equals("invalid arguments count", w.argCount(), 1);

    const FormatterConfig::Wrapper p1=w.param(0);
    tut::ensure("invalid type", p1.isArgument() );
    tut::ensure_equals("invalid argument value", p1.argument(), exp);
  }

  void testNumberThrow(const string &num) const
  {
    testThrow("fun("+num+")", "non-number has been accepted");
  }
  // TODO: consider use (if possible) 'trim' function from boost::algorithm
  string trim(const string &str) const
  {
    const string::size_type pos1=str.find_first_not_of(' ');
    const string::size_type pos2=str.find_last_not_of(' ');
    const string::size_type from=(pos1!=string::npos) ? pos1   : 0;
    const string::size_type to  =(pos2!=string::npos) ? pos2+1 : str.length();
    const string            out(str.begin()+from, str.begin()+to);
    return out;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Preprocessor/FunctionParser");
} // unnamed namespace


namespace tut
{

// test default parsing
template<>
template<>
void testObj::test<1>(void)
{
  const FunctionParser           fp("");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isValue() );
}

// test value() direct usage
template<>
template<>
void testObj::test<2>(void)
{
  const FunctionParser           fp("value()");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isValue() );
}

// test function with value() argument
template<>
template<>
void testObj::test<3>(void)
{
  const FunctionParser           fp("fun(value())");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type", p1.isValue() );
}

// test function with constant value parameter
template<>
template<>
void testObj::test<4>(void)
{
  const FunctionParser           fp("fun(`blah`)");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type", p1.isArgument() );
  ensure_equals("invalid argument value", p1.argument(), "blah");
}

// test function with multiple arguments
template<>
template<>
void testObj::test<5>(void)
{
  const FunctionParser           fp("fun(`foo`,value())");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 2);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type 0", p1.isArgument() );
  ensure_equals("invalid argument value 0", p1.argument(), "foo");

  const FormatterConfig::Wrapper p2=w.param(1);
  ensure("invalid type 1", p2.isValue() );
}

// test function w/o arguments
template<>
template<>
void testObj::test<6>(void)
{
  const FunctionParser           fp("fun()");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 0);
}

// test function taking function as an argument
template<>
template<>
void testObj::test<7>(void)
{
  const FunctionParser           fp("fun(other(`ppp`))");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type 0", p1.isFunction() );
  ensure_equals("invalid function name 0", p1.name(), "other");
  ensure_equals("invalid arguments count 0", p1.argCount(), 1);

  const FormatterConfig::Wrapper p2=p1.param(0);
  ensure("invalid type 1", p2.isArgument() );
  ensure_equals("invalid argument value 1", p2.argument(), "ppp");
}

// test multi function, mutli value
template<>
template<>
void testObj::test<8>(void)
{
  const FunctionParser           fp("fun(other(`ppp`),value(),misc(value()))");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 3);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type 0", p1.isFunction() );
  ensure_equals("invalid function name 0", p1.name(), "other");
  ensure_equals("invalid arguments count 0", p1.argCount(), 1);

  const FormatterConfig::Wrapper p2=p1.param(0);
  ensure("invalid type 1", p2.isArgument() );
  ensure_equals("invalid argument value 1", p2.argument(), "ppp");

  const FormatterConfig::Wrapper p3=w.param(1);
  ensure("invalid type 2", p3.isValue() );

  const FormatterConfig::Wrapper p4=w.param(2);
  ensure("invalid type 3", p4.isFunction() );
  ensure_equals("invalid function name 3", p4.name(), "misc");
  ensure_equals("invalid arguments count 3", p4.argCount(), 1);

  const FormatterConfig::Wrapper p5=p4.param(0);
  ensure("invalid type 4", p5.isValue() );
}

// test error on string as a root
template<>
template<>
void testObj::test<9>(void)
{
  testThrow("`str`", "didn't throw on string as a root element");
}

// test spaces
template<>
template<>
void testObj::test<10>(void)
{
  const FunctionParser           fp("  fun  (  other (  `ppp` ) )  ");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type 0", p1.isFunction() );
  ensure_equals("invalid function name 0", p1.name(), "other");
  ensure_equals("invalid arguments count 0", p1.argCount(), 1);

  const FormatterConfig::Wrapper p2=p1.param(0);
  ensure("invalid type 1", p2.isArgument() );
  ensure_equals("invalid argument value 1", p2.argument(), "ppp");
}

// test spaces inside the string
template<>
template<>
void testObj::test<11>(void)
{
  const FunctionParser           fp("fun(` bl - ah  `)");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type", p1.isArgument() );
  ensure_equals("invalid argument value", p1.argument(), " bl - ah  ");
}

// test extra brackets
template<>
template<>
void testObj::test<12>(void)
{
  const FunctionParser           fp("((value()))");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isValue() );
}

// test comma in quoted string
template<>
template<>
void testObj::test<13>(void)
{
  const FunctionParser           fp("fun(`foo,bar`)");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type", p1.isArgument() );
  ensure_equals("invalid argument value", p1.argument(), "foo,bar");
}

// test error on non-function as a root
template<>
template<>
void testObj::test<14>(void)
{
  testThrow("xyz", "didn't throw on non-function as a root element");
}

// test errro on invalid f-name
template<>
template<>
void testObj::test<15>(void)
{
  testThrow("xy$()", "didn't throw on invalid function name");
}

// test error on stirng w/o quotes
template<>
template<>
void testObj::test<16>(void)
{
  testThrow("xy(abc)", "didn't throw on string w/o quotes");
}

// test error on non-closed bracket
template<>
template<>
void testObj::test<17>(void)
{
  testThrow("xy(`abc`", "didn't throw on missing bracket");
}

// test error on non-closed quote
template<>
template<>
void testObj::test<18>(void)
{
  testThrow("xy(`abc)", "didn't throw on missing enclosing quote");
}

// test error on invalid brackets
template<>
template<>
void testObj::test<19>(void)
{
  testThrow("xy((`abc`))", "didn't throw on invalid brackets");
}

// test error on space in function name
template<>
template<>
void testObj::test<20>(void)
{
  testThrow("x y(`abc`)", "didn't throw on space in function name");
}

// test value() with spaces
template<>
template<>
void testObj::test<21>(void)
{
  const FunctionParser           fp(" value ( ) ");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isValue() );
}

// test error on space in value() function name
template<>
template<>
void testObj::test<22>(void)
{
  testThrow("val ue()", "didn't throw on space in value function name");
}

// test error when value() has arguments
template<>
template<>
void testObj::test<23>(void)
{
  testThrow("value(`arg`)", "didn't throw on argument to value()");
}

// test valueabc() - should be normal function
template<>
template<>
void testObj::test<24>(void)
{
  const FunctionParser           fp("valueabc()");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
}

// test camel-cased function names
template<>
template<>
void testObj::test<25>(void)
{
  const FunctionParser           fp("myFunction()");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
}

// test numbers in function names
template<>
template<>
void testObj::test<26>(void)
{
  const FunctionParser           fp("myFunction666()");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
}

// test error when funciton begins with upper case letter
template<>
template<>
void testObj::test<27>(void)
{
  testThrow("Invalid()", "didn't throw on upper-case as a first function name letter");
}

// test error when funciton begins with number
template<>
template<>
void testObj::test<28>(void)
{
  testThrow("6oops()", "didn't throw on number as a first function name char");
}

// test function with constant number parameter (with the quotes)
template<>
template<>
void testObj::test<29>(void)
{
  const FunctionParser           fp("fun(`42`)");
  const FormatterConfig::Wrapper w=fp.getConfig().get();
  ensure("invalid type", w.isFunction() );
  ensure_equals("invalid function name", w.name(), "fun");
  ensure_equals("invalid arguments count", w.argCount(), 1);

  const FormatterConfig::Wrapper p1=w.param(0);
  ensure("invalid type", p1.isArgument() );
  ensure_equals("invalid argument value", p1.argument(), "42");
}

// test function with constant number parameter (without the quotes)
template<>
template<>
void testObj::test<30>(void)
{
  testNumber("42");
}

// test function with constant fp-number parameter (without the quotes)
template<>
template<>
void testObj::test<31>(void)
{
  testNumber("4.5");
}

// test negative number w/o quotes
template<>
template<>
void testObj::test<32>(void)
{
  testNumber("-42");
}

// test negative fp-number w/o quotes
template<>
template<>
void testObj::test<33>(void)
{
  testNumber("-4.5");
}

// test signed, positive number w/o quotes
template<>
template<>
void testObj::test<34>(void)
{
  testNumber("+42");
}

// test signed, positive fp-number w/o quotes
template<>
template<>
void testObj::test<35>(void)
{
  testNumber("+4.5");
}

// test fp w/o quotes, w/o leading zero
template<>
template<>
void testObj::test<36>(void)
{
  testNumber(".5");
}

// test fp w/o quotes, w/o trailing zero
template<>
template<>
void testObj::test<37>(void)
{
  testNumber("2.");
}

// test signed fp w/o quotes, w/o leading zero
template<>
template<>
void testObj::test<38>(void)
{
  testNumber("+.5");
}

// test signed fp w/o quotes, w/o trailing zero
template<>
template<>
void testObj::test<39>(void)
{
  testNumber("+2.");
}

// test negative fp w/o quotes, w/o leading zero
template<>
template<>
void testObj::test<40>(void)
{
  testNumber("-.5");
}

// test negative fp w/o quotes, w/o trailing zero
template<>
template<>
void testObj::test<41>(void)
{
  testNumber("-2.");
}

// test random number w/o quotes, with spaces around
template<>
template<>
void testObj::test<42>(void)
{
  testNumber(" -4.25 ");
}

// test number with space inbetween
template<>
template<>
void testObj::test<43>(void)
{
  testNumberThrow("-4 .2");
}

// test number with sign in wrong place
template<>
template<>
void testObj::test<44>(void)
{
  testNumberThrow("4-2");
}

// test number with multiple dots
template<>
template<>
void testObj::test<45>(void)
{
  testNumberThrow("4.2.1");
}

// test multiple signs
template<>
template<>
void testObj::test<46>(void)
{
  testNumberThrow("--4.25");
}

} // namespace tut
