/*
 * viaCollection.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "System/Math/compareFP.hpp"
#include "Algo/Diff/viaCollection.hpp"

using namespace Algo::Diff;

namespace
{

struct TestType
{
  TestType(int v):
    v_(v)
  {
    assert(v_>=0);
    assert(v_<=9);
  }

  int v_;
}; // struct TestType

Similarity compare(const TestType &t1, const TestType &t2)
{
  return 1-std::fabs(t1.v_-t2.v_)/9;
} // compare()

struct TestClass
{
  typedef std::vector<TestType>          Vec;
  typedef Base::NonEmptyVector<TestType> NEVec;

  template<typename T>
  void testCmp(const T &c1, const T &c2, double expected) const
  {
    const Similarity s=Algo::Diff::compare(c1, c2);
    if( !System::Math::compareFP<double>(s.get(), expected, 3) )
      tut::ensure_equals("invalid similarity", s.get(), expected);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/Diff/viaCollection");
} // unnamed namespace


namespace tut
{

// self test
template<>
template<>
void testObj::test<1>(void)
{
  const Similarity s  =compare( TestType(3), TestType(4) );
  const double     exp=1-1.0/9;
  if( !System::Math::compareFP<double>(s.get(), exp, 3) )
    tut::ensure_equals("invalid similarity - self test failed", s.get(), exp);
}

// compare identical vectors
template<>
template<>
void testObj::test<2>(void)
{
  Vec v1;
  v1.push_back(5);
  v1.push_back(5);
  Vec v2=v1;
  testCmp(v1, v2, 1);
}

// compare non-empty vectors
template<>
template<>
void testObj::test<3>(void)
{
  NEVec v1(5);
  v1.push_back(5);
  NEVec v2=v1;
  testCmp(v1, v2, 1);
}

// compare different vectors of the same size
template<>
template<>
void testObj::test<4>(void)
{
  Vec v1;
  v1.push_back(5);
  v1.push_back(0);
  Vec v2;
  v2.push_back(5);
  v2.push_back(9);
  testCmp(v1, v2, 0.5);
}

// test different vectors of different sizes
template<>
template<>
void testObj::test<5>(void)
{
  Vec v1;
  v1.push_back(5);
  v1.push_back(0);
  Vec v2;
  v2.push_back(5);
  v2.push_back(9);
  v2.push_back(4);
  testCmp(v1, v2, 2.0/5);
}

// test comapring empty collections
template<>
template<>
void testObj::test<6>(void)
{
  Vec v1;
  Vec v2;
  testCmp(v1, v2, 1);
}

// test comapring when first collection is non-empty collections
template<>
template<>
void testObj::test<7>(void)
{
  Vec v1;
  v1.push_back(5);
  Vec v2;
  testCmp(v1, v2, 0);
}

// test comapring when second collection is non-empty collections
template<>
template<>
void testObj::test<8>(void)
{
  Vec v1;
  Vec v2;
  v2.push_back(5);
  testCmp(v1, v2, 0);
}

} // namespace tut
