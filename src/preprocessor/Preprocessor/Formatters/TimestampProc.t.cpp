/*
 * TimestampProc.t.cpp
 *
 */
#include <tut.h>

#include "Preprocessor/Formatters/TimestampProc.hpp"
#include "Preprocessor/Formatters/TestBase.t.hpp"

using namespace std;
using namespace Preprocessor::Formatters;

namespace
{

struct TestClass: public TestBase<TimestampProc>
{
  TestClass(void)
  {
    a_->set(29951133);  // date -d "@29951133" "+%Y.%m.%d %H:%M:%S" # 1970.12.13 16:45:33
    b_->set(108184);    // date -d   "@108184" "+%Y.%m.%d %H:%M:%S" # 1970.01.02 07:03:04
  }

  void testTs(ValuePtrNN v, const string &format, const string &expected) const
  {
    TimestampProc tsp(v, format);
    tut::ensure_equals("invalid string", tsp.exec(), expected);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Preprocessor/Formatters/TimestampProc");
} // unnamed namespace


namespace tut
{

// test invalid format
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    TimestampProc ts(a_, "%Q");     // should throw
    fail("c-tor didn't throw on invalid format");
  }
  catch(const TimestampProc::ExceptionInvalidFormat&)
  {
    // this is expected
  }
}

// test trailing %
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    TimestampProc ts(a_, "xx%");    // should throw
    fail("c-tor didn't throw on invalid format");
  }
  catch(const TimestampProc::ExceptionInvalidFormat&)
  {
    // this is expected
  }
}

// test empty string
template<>
template<>
void testObj::test<3>(void)
{
  testTs(a_, "", "");
}

// test some string w/o special sequences
template<>
template<>
void testObj::test<4>(void)
{
  testTs(a_, "hello test!", "hello test!");
}

// test '%' sign
template<>
template<>
void testObj::test<5>(void)
{
  testTs(a_, "%%", "%");
}

// test year
template<>
template<>
void testObj::test<6>(void)
{
  testTs(a_, "%Y", "1970");
}

// test 2-digit month
template<>
template<>
void testObj::test<7>(void)
{
  testTs(a_, "%m", "12");
}

// test 1-digit month
template<>
template<>
void testObj::test<8>(void)
{
  testTs(b_, "%m", "01");
}

// test 2-digit day
template<>
template<>
void testObj::test<9>(void)
{
  testTs(a_, "%d", "13");
}

// test 1-digit day
template<>
template<>
void testObj::test<10>(void)
{
  testTs(b_, "%d", "02");
}

// test 2-digit hour (24h)
template<>
template<>
void testObj::test<11>(void)
{
  testTs(a_, "%H", "16");
}

// test 1-digit hour
template<>
template<>
void testObj::test<12>(void)
{
  testTs(b_, "%H", "07");
}

// test 2-digit minute
template<>
template<>
void testObj::test<13>(void)
{
  testTs(a_, "%M", "45");
}

// test 1-digit minute
template<>
template<>
void testObj::test<14>(void)
{
  testTs(b_, "%M", "03");
}

// test 2-digit second
template<>
template<>
void testObj::test<15>(void)
{
  testTs(a_, "%S", "33");
}

// test 1-digit second
template<>
template<>
void testObj::test<16>(void)
{
  testTs(b_, "%S", "04");
}

// test sample, more complex, format
template<>
template<>
void testObj::test<17>(void)
{
  testTs(b_, "%Y.%m.%d %H:%M:%S", "1970.01.02 07:03:04");
}

// test if "%%%" is an error
template<>
template<>
void testObj::test<18>(void)
{
  try
  {
    TimestampProc ts(a_, "%%%");    // should throw
    fail("c-tor didn't throw on invalid format");
  }
  catch(const TimestampProc::ExceptionInvalidFormat&)
  {
    // this is expected
  }
}

// test if space between % and format symbol fails
template<>
template<>
void testObj::test<19>(void)
{
  try
  {
    TimestampProc ts(a_, "% Y");    // should throw
    fail("c-tor didn't throw on invalid format");
  }
  catch(const TimestampProc::ExceptionInvalidFormat&)
  {
    // this is expected
  }
}

} // namespace tut
