/*
 * treeSumFP.t.cpp
 *
 */
#include <tut.h>
#include <vector>

#include "System/Math/detail/treeSumFP.hpp"
#include "System/Math/TestHelpers.t.hpp"

using namespace System::Math::detail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Math/detail/treeSumFP");
}


namespace tut
{

// test summing smaple array
template<>
template<>
void testObj::test<1>(void)
{
  double       tab[4]={1.1, 1.9, 42, 1000}; // intentionaly do not use last element
  const double res   =treeSumFP<double>(&tab[0], &tab[2]+1);
  ensure_range("invalid sum", res, 45, 0.01);
}

// test sum of 1 element array
template<>
template<>
void testObj::test<2>(void)
{
  double        tab[2]={1.1, 1000};         // intentionaly do not use last element
  const double res    =treeSumFP<double>(&tab[0], &tab[0]+1);
  ensure_range("invalid sum", res, 1.1, 0.01);
}

// summing of 2 element array
template<>
template<>
void testObj::test<3>(void)
{
  std::vector<double> d;
  d.push_back(1.2);
  d.push_back(2.2);
  const double res=treeSumFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 3.4, 0.01);
}

// summing of 3 element array
template<>
template<>
void testObj::test<4>(void)
{
  std::vector<double> d;
  d.push_back(1.2);
  d.push_back(2.2);
  d.push_back(3.2);
  const double res=treeSumFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 6.6, 0.01);
}

// summing of 4 element array
template<>
template<>
void testObj::test<5>(void)
{
  std::vector<double> d;
  d.push_back(1.2);
  d.push_back(2.2);
  d.push_back(3.2);
  d.push_back(4.2);
  const double res=treeSumFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 10.8, 0.01);
}

// summing of 5 element array
template<>
template<>
void testObj::test<6>(void)
{
  std::vector<double> d;
  d.push_back(1.2);
  d.push_back(2.2);
  d.push_back(3.2);
  d.push_back(4.2);
  d.push_back(5.2);
  const double res=treeSumFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 16.0, 0.01);
}

// summing array of equal elements
template<>
template<>
void testObj::test<7>(void)
{
  std::vector<double> d;
  d.push_back(1.2);
  d.push_back(1.2);
  const double res=treeSumFP<double>( d.begin(), d.end() );
  ensure_range("invalid sum", res, 2.4, 0.01);
}

} // namespace tut
