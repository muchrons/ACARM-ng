/*
 * ObservingSetBase.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "Base/ObservingSetBase.hpp"

using namespace Base;

namespace
{
struct TestOSet: public ObservingSetBase<int*, TestOSet>
{
  static bool isUnused(int *e)
  {
    assert(e!=NULL);
    return *e==-1;      // magic value for unused
  }
  static bool areEqual(int *a, int *b)
  {
    assert(a!=NULL);
    assert(b!=NULL);
    return *a==*b;
  }
}; // struct TestOSet

struct TestClass
{
  typedef TestOSet::iterator       Iter;
  typedef TestOSet::const_iterator ConstIter;

  TestClass(void)
  {
    for(int i=0; i<2; ++i)
    {
      data_[i]=10-i;
      d1_.add(&data_[i]);
    }
  }

  const TestOSet &mkc(TestOSet &o) const
  {
    return o;
  }

  int size(const TestOSet &s) const
  {
    int cnt=0;
    for(ConstIter it=s.begin(); it!=s.end(); ++it)
      ++cnt;
    return cnt;
  }

  int      data_[3];
  TestOSet empty_;
  TestOSet d1_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ObservingSetBase");
} // unnamed namespace


namespace tut
{

// check empty collection
template<>
template<>
void testObj::test<1>(void)
{
  ensure("collection not empty", empty_.begin()==empty_.end() );
}

// check non-empty collection
template<>
template<>
void testObj::test<2>(void)
{
  ensure("collection empty", d1_.begin()!=d1_.end() );
}

// check empty collection (const version)
template<>
template<>
void testObj::test<3>(void)
{
  ensure("const collection not empty", mkc(empty_).begin()==mkc(empty_).end() );
}

// check non-empty collection (const version)
template<>
template<>
void testObj::test<4>(void)
{
  ensure("const collection empty", mkc(d1_).begin()!=mkc(d1_).end() );
}

// test adding new element to sequence
template<>
template<>
void testObj::test<5>(void)
{
  empty_.add(&data_[0]);
  // check
  Iter it=empty_.begin();
  ensure("adding to empty collection failed", it!=empty_.end() );
  ensure("NULL pointer returned", *it!=NULL );
  ensure_equals("invalid value", **it, 10);
  ++it;
  ensure("too many elements in collection", it==empty_.end() );
}

// test different elements
template<>
template<>
void testObj::test<6>(void)
{
  // 1st
  Iter it=d1_.begin();
  {
    ensure("adding to empty collection failed", it!=d1_.end() );
    ensure("ptr 1 is NULL", *it!=NULL );
    ensure_equals("invalid value 1", **it, 10);
    ++it;
  }
  // 2nd
  {
    ensure("adding to empty collection failed", it!=d1_.end() );
    ensure("ptr 1 is NULL", *it!=NULL );
    ensure_equals("invalid value 2", **it, 9);
    ++it;
  }
  // end
  {
    ensure("too many elements in collection", it==d1_.end() );
  }
}

// test re-inserting the same element
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("invalid initial size", size(d1_), 2);
  d1_.add(&data_[0]);
  ensure_equals("invalid size after re-adding the same element", size(d1_), 2);
}

// test prune on empty set
template<>
template<>
void testObj::test<8>(void)
{
  ensure_equals("invalid initial size", size(empty_), 0);
  empty_.prune();
  ensure_equals("invalid size after pruning", size(empty_), 0);
}

// test size after add()
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("invalid initial size", size(empty_), 0);
  empty_.add(&data_[1]);
  ensure_equals("invalid size after 1st add", size(empty_), 1);
  empty_.add(&data_[0]);
  ensure_equals("invalid size after 2nd add", size(empty_), 2);
}

// test prune on non-empty set
template<>
template<>
void testObj::test<10>(void)
{
  ensure_equals("invalid initial size", size(d1_), 2);
  d1_.prune();
  ensure_equals("invalid size after pruning", size(d1_), 2);
}

// test pruning set with NULLs
template<>
template<>
void testObj::test<11>(void)
{
  data_[2]=42;
  d1_.add(&data_[2]);
  ensure_equals("invalid initial size", size(d1_), 3);
  // null some pointers
  data_[0]=-1;
  data_[1]=-1;
  // prune
  d1_.prune();

  // check
  ensure_equals("invalid size after pruning", size(d1_), 1);
  ensure("pointer is NULL", *d1_.begin()!=NULL );
  ensure_equals("invalid elements removed", **d1_.begin(), 42);
}

} // namespace tut
