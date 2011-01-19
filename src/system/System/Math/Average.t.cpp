/*
 * Average.t.cpp
 *
 */
#include <tut.h>
#include <list>
#include <vector>

#include "System/Math/Average.hpp"
#include "System/Math/TestHelpers.t.hpp"

using namespace std;
using namespace System::Math;

namespace
{

struct TestClass
{
  template<typename FP, typename TConstIter>
  void testAvg(const TConstIter begin, const TConstIter end,
               const FP avg, const FP stdDev, const size_t size) const
  {
    const Average<FP> a(begin, end);
    tut::ensure_equals("invalid number of elements", a.count(), size);
    ensure_range("invalid average",   a.avg(),    avg,    0.01);
    ensure_range("invalid std. dev.", a.stdDev(), stdDev, 0.01);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/Math/Average");
}


namespace tut
{

// test list and floats
template<>
template<>
void testObj::test<1>(void)
{
  list<float> l;
  l.push_back(2);
  l.push_back(4);
  l.push_back(5);
  testAvg( l.begin(), l.end(), 3.666, 1.527, 3 );
}

// test vector and doubles
template<>
template<>
void testObj::test<2>(void)
{
  std::vector<double> v;
  v.push_back(2);
  v.push_back(4);
  v.push_back(5);
  testAvg( v.begin(), v.end(), 3.666, 1.527, 3 );
}

// test single element
template<>
template<>
void testObj::test<3>(void)
{
  std::vector<double> v;
  v.push_back(2);
  testAvg( v.begin(), v.end(), 2.0, 0.0, 1 );
}

// test the the same values
template<>
template<>
void testObj::test<4>(void)
{
  std::vector<double> v;
  v.push_back(2);
  v.push_back(2);
  v.push_back(2);
  v.push_back(2);
  testAvg( v.begin(), v.end(), 2.0, 0.0, 4 );
}

// test exception on empty set
template<>
template<>
void testObj::test<5>(void)
{
  std::vector<double> v;
  try
  {
    Average<double> a( v.begin(), v.end() );
    fail("c-tor didn't throw on empty range");
  }
  catch(const ExceptionInvalidRange &)
  {
    // this is expected
  }
}

// test average of natural numbers
template<>
template<>
void testObj::test<6>(void)
{
  std::vector<int> v;
  v.push_back(3);
  v.push_back(-1);
  v.push_back(2);
  testAvg( v.begin(), v.end(), 1.332, 2.082, 3 );
}

} // namespace tut

