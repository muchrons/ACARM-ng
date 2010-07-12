/*
 * ObservingPairSet.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Base/ObservingPairSet.hpp"

using namespace std;
using namespace tut;
using namespace Base;

namespace
{
struct TestClass
{
  typedef std::string                 Type;
  typedef ObservingPairSet<Type, int> OSet;

  typedef OSet::iterator       Iter;
  typedef OSet::const_iterator ConstIter;

  TestClass(void):
    ptr1_( mkPtr("hello") ),
    ptr2_( mkPtr("HAL 9000") )
  {
    d1_.add(ptr1_, 1);
    d1_.add(ptr2_, 2);
  }

  OSet::Data::SharedPtr mkPtr(const char *str) const
  {
    return OSet::Data::SharedPtr( new string(str) );
  }

  int size(const OSet &s) const
  {
    int cnt=0;
    for(ConstIter it=s.begin(); it!=s.end(); ++it)
      ++cnt;
    return cnt;
  }

  OSet::Data::SharedPtr ptr1_;
  OSet::Data::SharedPtr ptr2_;

  OSet empty_;
  OSet d1_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/ObservingPairSet");
} // unnamed namespace


namespace tut
{

// test adding new element to sequence
template<>
template<>
void testObj::test<1>(void)
{
  empty_.add(ptr1_, 42);
  // check
  Iter it=empty_.begin();
  ensure("adding to empty collection failed", it!=empty_.end() );
  OSet::Data::SharedPtr tmp=it->ptr_.lock();
  ensure("NULL pointer returned", tmp.get()!=NULL );
  ensure_equals("invalid value", *tmp, "hello");
  ensure_equals("invalid data", it->data_, 42);
  ++it;
  ensure("too many elements in collection", it==empty_.end() );
}

// test prune on non-empty set
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid initial size", size(d1_), 2);
  d1_.prune();
  ensure_equals("invalid size after pruning", size(d1_), 2);
}

// test pruning set with NULLs
template<>
template<>
void testObj::test<3>(void)
{
  OSet::Data::SharedPtr ptr3=mkPtr("xyz");
  d1_.add(ptr3, 11);
  ensure_equals("invalid initial size", size(d1_), 3);
  // null some pointers
  ptr1_=mkPtr("removed");
  ptr2_=mkPtr("removed");
  // prune
  d1_.prune();

  // check
  ensure_equals("invalid size after pruning", size(d1_), 1);
  OSet::Data::SharedPtr tmp=d1_.begin()->ptr_.lock();
  ensure("pointer is NULL", tmp.get()!=NULL );
  ensure_equals("invalid elements removed", *tmp, *ptr3);
  ensure_equals("invalida data", d1_.begin()->data_, 11);
}

// test test adding entry with different data (additional data must be
// compared as well).
template<>
template<>
void testObj::test<4>(void)
{
  empty_.add(ptr1_, 1);
  empty_.add(ptr1_, 2);
  ensure_equals("invalid initial size", size(d1_), 2);
}

} // namespace tut
