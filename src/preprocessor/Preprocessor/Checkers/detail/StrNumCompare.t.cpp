/*
 * StrNumCompare.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <utility>

#include "Preprocessor/Checkers/detail/StrNumCompare.hpp"

using namespace Preprocessor::Checkers::detail;

namespace
{

// global entries to comapre with
typedef std::pair<double, double>           PairNum;
typedef std::pair<std::string, std::string> PairStr;
PairNum g_numExp;
PairStr g_strExp;

// comparator that saves arguments
struct CmpStub
{
  bool operator()(const double left, const double right) const
  {
    tut::ensure_equals("invalid left argument",  left,  g_numExp.first);
    tut::ensure_equals("invalid right argument", right, g_numExp.second);
    return false;
    //return left < right;
  }

  bool operator()(const std::string &left, const std::string &right) const
  {
    tut::ensure_equals("invalid left argument",  left,  g_strExp.first);
    tut::ensure_equals("invalid right argument", right, g_strExp.second);
    return true;
    //return left < right;
  }
}; // struct CmpStub


// test comparator instance
struct TestCmp: public StrNumCompare<CmpStub>
{
  TestCmp(const std::string &str):
    StrNumCompare<CmpStub>("testcomparer", str)
  {
  }
}; // struct TestCmp


struct TestClass
{
  TestClass(void)
  {
    g_strExp=PairStr("NOT SET", "NOT SET");
    g_numExp=PairNum(-666.666,  -777.777 );
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Checkers/detail/StrNumCompare");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  TestCmp ce("abc");
}

// test comparison for a non-numbers
template<>
template<>
void testObj::test<2>(void)
{
  const TestCmp ce("abc");
  g_strExp=PairStr("Z", "abc");
  ensure("comparison failed", ce.check("Z") );
}

// test comparison for a numbers
template<>
template<>
void testObj::test<3>(void)
{
  const TestCmp ce("42");
  g_numExp=PairNum(5, 42);
  ensure("comparison didn't failed", ce.check("5")==false );
}

// test when c-tor argument is not a number, but method paramter is a number
template<>
template<>
void testObj::test<4>(void)
{
  const TestCmp ce("test");
  g_strExp=PairStr("10", "test");
  ensure("comparison failed", ce.check("10") );
}

// test when c-tor argument is not a number, but method parameter is a number
template<>
template<>
void testObj::test<5>(void)
{
  const TestCmp ce("50");
  g_strExp=PairStr("test", "50");
  ensure("comparison failed", ce.check("test") );
}

// test comaprison of empty strings
template<>
template<>
void testObj::test<6>(void)
{
  const TestCmp ce("");
  g_strExp=PairStr("", "");
  ensure("comparison failed", ce.check("") );
}

} // namespace tut
