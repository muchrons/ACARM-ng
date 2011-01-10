/*
 * TruncatingQueue.t.cpp
 *
 */
#include <tut.h>
#include <stack>
#include <string>

#include "Base/TruncatingQueue.hpp"

using namespace std;
using namespace Base;

namespace
{
struct TestClass
{
  typedef std::string                       Type;
  typedef std::stack<Type>                  InternalQueue;
  typedef TruncatingQueue<InternalQueue, 4> Queue;

  Queue q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/TruncatingQueue");
} // unnamed namespace


namespace tut
{

// test empty collection's size
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("collection not empty", q_.size(), 0u );
}

// test adding new element to collection
template<>
template<>
void testObj::test<2>(void)
{
  q_.push("alice has a cat");
  ensure_equals("element not added", q_.size(), 1u );
}

// test top()
template<>
template<>
void testObj::test<3>(void)
{
  q_.push("abc");
  ensure_equals("invalid element", q_.top(), "abc");
  ensure_equals("invalid size", q_.size(), 1u );
}

// test pop()
template<>
template<>
void testObj::test<4>(void)
{
  q_.push("abc");
  ensure_equals("invalid element", q_.pop(), "abc");
  ensure_equals("invalid size", q_.size(), 0u );
}

// test adding to full queue
template<>
template<>
void testObj::test<5>(void)
{
  q_.push("a");
  q_.push("b");
  q_.push("c");
  q_.push("d");
  q_.push("e");         // this element makes queue full
  // check size
  ensure_equals("invalid size", q_.size(), 4u );
  // check contet (it looks funny here, since this is stack,
  // element to be removed is on top - so not the oldest is removed)
  ensure_equals("invalid element 1", q_.pop(), "e");
  ensure_equals("invalid element 2", q_.pop(), "c");
  ensure_equals("invalid element 3", q_.pop(), "b");
  ensure_equals("invalid element 4", q_.pop(), "a");
}

// test for maxSize()
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("invalid maxumum size", q_.maxSize(), 4u);
}

} // namespace tut
