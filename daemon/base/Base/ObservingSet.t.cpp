/*
 * ObservingSet.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Base/ObservingSet.hpp"

using namespace std;
using namespace tut;
using namespace Base;

namespace
{
struct TestClass
{
  typedef std::string        Type;
  typedef ObservingSet<Type> OSet;

  typedef OSet::iterator       Iter;
  typedef OSet::const_iterator ConstIter;

  TestClass(void):
    ptr1_( mkPtr("hello") ),
    ptr2_( mkPtr("HAL 9000") )
  {
    d1_.add(ptr1_);
    d1_.add(ptr2_);
  }

  OSet::SharedPtr mkPtr(const char *str) const
  {
    return OSet::SharedPtr( new string(str) );
  }

  const OSet &qc(OSet &o) const
  {
    return o;
  }

  int size(const OSet &s) const
  {
    int cnt=0;
    for(ConstIter it=s.begin(); it!=s.end(); ++it)
      ++cnt;
    return cnt;
  }

  OSet::SharedPtr ptr1_;
  OSet::SharedPtr ptr2_;

  OSet empty_;
  OSet d1_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ObservingSet");
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

// check empty collection
template<>
template<>
void testObj::test<3>(void)
{
  ensure("const collection not empty", qc(empty_).begin()==qc(empty_).end() );
}

// check non-empty collection
template<>
template<>
void testObj::test<4>(void)
{
  ensure("const collection empty", qc(d1_).begin()!=qc(d1_).end() );
}

// test adding new element to sequence
template<>
template<>
void testObj::test<5>(void)
{
  empty_.add(ptr1_);
  // check
  Iter it=empty_.begin();
  ensure("adding to empty collection failed", it!=empty_.end() );
  OSet::SharedPtr tmp=it->lock();
  ensure("NULL pointer returned", tmp.get()!=NULL );
  ensure_equals("invalid value", *tmp, "hello");
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
    OSet::SharedPtr tmp=it->lock();
    ensure("ptr 1 is NULL", tmp.get()!=NULL );
    ensure_equals("invalid value 1", *tmp, "hello");
    ++it;
  }
  // 2nd
  {
    ensure("adding to empty collection failed", it!=d1_.end() );
    OSet::SharedPtr tmp=it->lock();
    ensure("ptr 1 is NULL", tmp.get()!=NULL );
    ensure_equals("invalid value 2", *tmp, "HAL 9000");
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
  d1_.add(ptr2_);
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

// test size()
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("invalid initial size", size(empty_), 0);
  empty_.add(ptr2_);
  ensure_equals("invalid size after 1st add", size(empty_), 1);
  empty_.add(ptr1_);
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
  OSet::SharedPtr ptr3=mkPtr("xyz");
  d1_.add(ptr3);
  ensure_equals("invalid initial size", size(d1_), 3);
  // null some pointers
  ptr1_=mkPtr("removed");
  ptr2_=mkPtr("removed");
  // prune
  d1_.prune();

  // check
  ensure_equals("invalid size after pruning", size(d1_), 1);
  OSet::SharedPtr tmp=d1_.begin()->lock();
  ensure("pointer is NULL", tmp.get()!=NULL );
  ensure_equals("invalid elements removed", *tmp, *ptr3);
}

} // namespace tut
