/*
 * Convert.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cmath>

#include "Commons/Convert.hpp"

using namespace std;
using namespace Commons;

namespace
{

struct TestClass
{
  template<typename T, typename F>
  void ensureThrow(const F &f) const
  {
    try
    {
      Convert::to<T>(f);    // should throw
      tut::fail("convertion didn't throw, as expected");
    }
    catch(const Convert::ExceptionConvertionError &)
    {
      // this is expected
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Convert");
} // unnamed namespace


namespace tut
{

// test converting int to string
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("int->string failed", Convert::to<string>(42), "42");
}

// convert negative int to string
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("negative int->string failed", Convert::to<string>(-42), "-42");
}

// convert double to string
template<>
template<>
void testObj::test<3>(void)
{
  ensure("double->string failed", Convert::to<string>(-4.2).find("-4.2")!=string::npos );
}

// conver string to double
template<>
template<>
void testObj::test<4>(void)
{
  ensure("string->double failed", fabs( Convert::to<double>("-4.2")-(-4.2) )<0.001 );
}

// convert string to integer
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("string->int failed", Convert::to<int>("42"), 42);
}

// convert string to negative integer
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("string->int failed", Convert::to<int>("-42"), -42);
}

// convert string to unsigned integer
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("string->unsigned int failed", Convert::to<unsigned int>("42"), 42);
}

// convert int to double
template<>
template<>
void testObj::test<8>(void)
{
  ensure("int->double failed", fabs( Convert::to<double>(42)-42 )<0.001 );
}

// test throw when converting double to int
template<>
template<>
void testObj::test<9>(void)
{
  ensureThrow<int, double>(4.2);
}

// test throw when non-number is being translated to one
template<>
template<>
void testObj::test<10>(void)
{
  ensureThrow<int, string>("4.2UGH!");
}

// test throw when converting negative number to unsigned type
template<>
template<>
void testObj::test<11>(void)
{
  ensureThrow<unsigned int, string>("-13");
}

// test converting the same value to the same value
template<>
template<>
void testObj::test<12>(void)
{
  ensure_equals("string->string failed", Convert::to<string, string>("1.2"), "1.2");
}

// test converting bigger integral type to smaller one
template<>
template<>
void testObj::test<13>(void)
{
  ensureThrow<char, long>(303);
}

// test no throwing when element fits the smaller type.
template<>
template<>
void testObj::test<14>(void)
{
  ensure_equals("convertion failed", Convert::to<char, long>(42), 42);
}

// test converting to unsigned type
template<>
template<>
void testObj::test<15>(void)
{
  ensure_equals("convertion failed", Convert::to<unsigned char, long>(42), 42);
}

// test throw when converting string (negative number) to unsigned number
template<>
template<>
void testObj::test<16>(void)
{
  ensureThrow<unsigned char, string>("-11");
}

} // namespace tut
