/*
 * arraySumFastFP.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <list>

#include "System/Math/arraySumFastFP.hpp"
#include "System/Math/TestHelpers.t.hpp"

using namespace System::Math;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Math/arraySumFastFP");
}


namespace tut
{

// test summing smaple array
template<>
template<>
void testObj::test<1>(void)
{
  const double tab[3]={1.1, 0.9, 42};
  const double res=arraySumFastFP<double>(&tab[0], &tab[2]+1);
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
    arraySumFastFP<double>( d.begin(), d.end() );
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
  const double res=arraySumFastFP<double>( l.begin(), l.end() );
  ensure_range("invalid sum", res, 7.6, 0.01);
}

} // namespace tut

