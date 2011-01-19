/*
 * arraySumPrecFP.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <list>

#include "System/Math/arraySumPrecFP.hpp"
#include "System/Math/TestHelpers.t.hpp"

using namespace System::Math;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Math/arraySumPrecFP");
}


namespace tut
{

// test summing smaple array
template<>
template<>
void testObj::test<1>(void)
{
  const double tab[3]={1.1, 0.9, 42};
  const double res=arraySumPrecFP<double>(&tab[0], &tab[2]+1);
  ensure_range("invalid sum", res, 44, 0.01);
}

// test exception on empty range
template<>
template<>
void testObj::test<2>(void)
{
  std::vector<double> d;
  try
  {
    arraySumPrecFP<double>( d.begin(), d.end() );
    fail("call didn't throw on empty range");
  }
  catch(const ExceptionInvalidRange &)
  {
    // this is expected
  }
}

// test iteration over a list
template<>
template<>
void testObj::test<3>(void)
{
  std::list<double> l;
  l.push_back(4.2);
  l.push_back(1.2);
  l.push_back(2.2);
  const double res=arraySumPrecFP<double>( l.begin(), l.end() );
  ensure_range("invalid sum", res, 7.6, 0.01);
}

// summ of a big array
template<>
template<>
void testObj::test<4>(void)
{
  std::vector<double> d;
  for(int i=0; i<10; ++i)
    d.push_back(1);
  const double res=arraySumPrecFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 10, 0.01);
}

// summ of one element
template<>
template<>
void testObj::test<5>(void)
{
  std::vector<double> d;
  d.push_back(6);
  const double res=arraySumPrecFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 6, 0.01);
}

// summ of two elements
template<>
template<>
void testObj::test<6>(void)
{
  std::vector<double> d;
  d.push_back(6);
  d.push_back(3);
  const double res=arraySumPrecFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 9, 0.01);
}

} // namespace tut

