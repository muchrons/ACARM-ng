/*
 * TimeoutQueue.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Base/TimeoutQueue.hpp"

using namespace std;
using namespace tut;
using namespace Base;

namespace
{
struct TestClass
{
  typedef std::string           Type;
  typedef TimeoutQueue<Type>    Queue;

  typedef Queue::iterator       Iter;
  typedef Queue::const_iterator ConstIter;

  TestClass(void)
  {
    q1_.update("hello", 10);
    q1_.update("world",  8);
  }

  // quick convertion to const
  const Queue &qc(Queue &q) const
  {
    return q;
  }

  int size(const Queue &q) const
  {
    int cnt=0;
    for(ConstIter it=q.begin(); it!=q.end(); ++it)
      ++cnt;
    return cnt;
  }

  Queue empty_;
  Queue q1_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/TimeoutQueue");
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
  ensure("collection empty", q1_.begin()!=q1_.end() );
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
  ensure("const collection empty", qc(q1_).begin()!=qc(q1_).end() );
}

// test adding new element to sequence
template<>
template<>
void testObj::test<5>(void)
{
  empty_.update("narf", 10);
  // check
  Iter it=empty_.begin();
  ensure("adding to empty collection failed", it!=empty_.end() );
  ensure_equals("invalid value", *it, "narf");
  ++it;
  ensure("too many elements in collection", it==empty_.end() );
}

// test adding other element
template<>
template<>
void testObj::test<6>(void)
{
  empty_.update("narf", 10);
  empty_.update("kszy", 15);

  // check

  // 1st
  Iter it=empty_.begin();
  ensure("adding to empty collection failed", it!=empty_.end() );
  ensure_equals("invalid value 1", *it, "narf");
  ++it;
  // 2nd
  ensure("adding to empty collection failed", it!=empty_.end() );
  ensure_equals("invalid value 2", *it, "kszy");
  ++it;
  // end
  ensure("too many elements in collection", it==empty_.end() );
}

// test updating timeout
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("invalid size/1", size(empty_), 0);
  empty_.update("old", 0);  // this should be timeouted strigh away
  empty_.update("abc", 9);  // some garbage
  ensure_equals("invalid size/2", size(empty_), 2);
  sleep(1);                 // now for sure!

  empty_.update("old", 6);  // add some time
  ensure_equals("invalid size/3", size(empty_), 2);
  empty_.prune();           // entry should not be removed
  ensure_equals("invalid size/4", size(empty_), 2);

  // check value
  ConstIter it=empty_.begin();
  ensure("element removed", it!=qc(empty_).end() );
  ensure_equals("invalid element 1", *it, "old");
  ++it;
  ensure("element removed", it!=qc(empty_).end() );
  ensure_equals("invalid element 2", *it, "abc");
}

// test removing timeouted elements
template<>
template<>
void testObj::test<8>(void)
{
  empty_.update("old1", 1);
  empty_.update("new1", 9);
  empty_.update("old2", 0);
  empty_.update("new2", 0);
  empty_.update("new2", 8); // update previous entry
  ensure_equals("invalid number of elements", size(empty_), 4);
  // wait for timeouts
  sleep(2);

  // do the evolution!
  empty_.prune();

  // check size
  ensure_equals("invalid number of elements after pruning", size(empty_), 2);
  // check content
  ConstIter it=empty_.begin();
  ensure_equals("invalid element 1", *it, "new1");
  ++it;
  ensure_equals("invalid element 2", *it, "new2");
}

// test updating timeout should make time longer by a given amount.
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("invalid size/1", size(empty_), 0);
  empty_.update("old", 0);  // this should be timeouted strigh away
  empty_.update("old", 1);  // now make it last longer, but not too long
  ensure_equals("invalid size/2", size(empty_), 1);
  sleep(2);                 // now for sure!

  empty_.prune();           // entry should now be removed
  ensure_equals("invalid size/4", size(empty_), 0);
}

// test explicit removal of element
template<>
template<>
void testObj::test<10>(void)
{
  empty_.update("e1", 2);
  empty_.update("e2", 9);
  empty_.update("e3", 3);
  ensure_equals("invalid number of elements", size(empty_), 3);

  // explicitly removed 2nd element
  {
    Iter it=empty_.begin();
    ++it;
    empty_.dismiss(it);
  }

  // check size
  ensure_equals("invalid number of elements after pruning", size(empty_), 2);
  // check content
  ConstIter it=empty_.begin();
  ensure_equals("invalid element 1", *it, "e1");
  ++it;
  ensure_equals("invalid element 1", *it, "e3");
}

} // namespace tut
