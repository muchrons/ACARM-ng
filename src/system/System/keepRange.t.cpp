/*
 * keepRange.t.cpp
 *
 */
#include <tut.h>
#include <cmath>

#include "System/keepRange.hpp"

namespace System
{

struct KeepRangeTestClass
{
};

} // namespace System


namespace tut
{
typedef System::KeepRangeTestClass TestClass;
typedef test_group<TestClass> factory;
typedef factory::object testObj;
} // namespace tut


namespace
{
tut::factory tf("System/keepRange");
}

using namespace System;

namespace tut
{

// test range match
template<>
template<>
void testObj::test<1>(void)
{
  ensure( keepRange(12, 1,100)==12 );
  const int v  =12;
  const int min=10;
  const int max=20;
  ensure( keepRange(v, min,max)==v );
}

// test lower bound
template<>
template<>
void testObj::test<2>(void)
{
  ensure( keepRange(10, 10,20)==10 );
  ensure( keepRange(9,  10,20)==10 );
}

// test upper bound
template<>
template<>
void testObj::test<3>(void)
{
  ensure( keepRange(20, 10,20)==20 );
  ensure( keepRange(21, 10,20)==20 );
}

// try if keepRange() will throw on error
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    keepRange(30, 30,29);       // should throw
    fail("keepRange() didn't throw, given incorrect range");
  }
  catch(const System::Exception&)
  {
    // this is expected
  }
}

// test converting values to matching range
template<>
template<>
void testObj::test<5>(void)
{
  const double in =10;
  const char   out=keepRange<char>(in);
  ensure(out==10);
}

// test converting when value does NOT fit
template<>
template<>
void testObj::test<6>(void)
{
  // test upper bound
  {
    const double in =321;
    const char   out=keepRange<char>(in);
    ensure(out==127);
  }
  // test lower bound
  {
    const double in =-321;
    const char   out=keepRange<char>(in);
    ensure(out==-128);
  }
}

// try converting from type with less precision
template<>
template<>
void testObj::test<7>(void)
{
  const char   in =66;
  const double out=keepRange<double>(in);
  ensure( fabs(out-in)<0.00001 );
}

// test converting real floating point
template<>
template<>
void testObj::test<9>(void)
{
  const double in =13.8;
  const int    out=keepRange<int>(in);
  ensure(out==13);
}

// test real, negative FP
template<>
template<>
void testObj::test<10>(void)
{
  const double in =-13.8;
  const int    out=keepRange<int>(in);
  ensure(out==-13);
}

} // namespace tut
