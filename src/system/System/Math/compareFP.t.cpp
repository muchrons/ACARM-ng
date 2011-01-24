/*
 * compareFP.t.cpp
 *
 */
#include <tut.h>

#include "System/Math/compareFP.hpp"

using namespace System::Math;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Math/compareFP");
}


namespace tut
{

// test comapring equal values
template<>
template<>
void testObj::test<1>(void)
{
  ensure("equal values not equal", compareFP(42.0, 42.0) );
}

// test comparing values with a given precision
template<>
template<>
void testObj::test<2>(void)
{
  const double diff=std::numeric_limits<double>::epsilon();
  ensure("equal values not equal", compareFP(1.0, 1.0+10*diff, 11) );
}

// test comparing different values
template<>
template<>
void testObj::test<3>(void)
{
  ensure("different values are equal", !compareFP(1.0, 1.1, 1) );
}

// test precision==0
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    compareFP(1.1, 2.2, 0);
    fail("operation didn't throw on prec=0");
  }
  catch(const ExceptionInvalidPrecision &)
  {
    // this is expected
  }
}

// try comapring floats
template<>
template<>
void testObj::test<5>(void)
{
  const float diff=std::numeric_limits<float>::epsilon();
  ensure("equal values not equal", compareFP<float>(1.0, 1.0+10*diff, 11) );
}

} // namespace tut

