/*
 * DivingStack.t.cpp
 *
 */
#include <tut.h>

#include "System/DivingStack.hpp"

using namespace System;

namespace
{
struct TestClass
{
  typedef DivingStack<int> DivSt;

  TestClass(void):
    ds_(10),
    cds_(ds_)
  {
  }

  DivSt        ds_;
  const DivSt &cds_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/DivingStack");
}

namespace tut
{

// test error when trying to create stack of size==0
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    DivSt ds(0);    // should throw
    fail("c-tor didn't throw on invalid maximum size");
  }
  catch(const DivSt::ExceptionInvalidMaximumSize &)
  {
    // this is expected
  }
}

// test maxSize()
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid maximum size", cds_.maxSize(), 10u);
}

// test size()
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid initial size", cds_.size(), 0u);
  ds_.push(666);
  ensure_equals("invalid size / 1", cds_.size(), 1u);
  ds_.push(13);
  ensure_equals("invalid size / 2", cds_.size(), 2u);
}

// test empty
template<>
template<>
void testObj::test<4>(void)
{
  ensure("stack not empty initially", cds_.empty() );
}

// test non-empty
template<>
template<>
void testObj::test<5>(void)
{
  ds_.push(42);
  ensure("empty stack after addition", cds_.empty()==false );
}

// test top()
template<>
template<>
void testObj::test<6>(void)
{
  ds_.push(11);
  ensure_equals("invalid element", ds_.top(), 11);
}

// test assignment to top()
template<>
template<>
void testObj::test<7>(void)
{
  ds_.push(11);
  ds_.top()=42;
  ensure_equals("invalid element", ds_.top(), 42);
}

// test const version of top()
template<>
template<>
void testObj::test<8>(void)
{
  ds_.push(11);
  ensure_equals("invalid element", cds_.top(), 11);
}

// test pop()
template<>
template<>
void testObj::test<9>(void)
{
  ds_.push(10);
  ds_.push(20);
  ds_.pop();
  ensure_equals("element nto removed", cds_.size(), 1u);
  ensure_equals("invalid element removed", cds_.top(), 10);
}

// test pushing element on the stack
template<>
template<>
void testObj::test<10>(void)
{
  ds_.push(123);
  ensure_equals("invalid number of elements", cds_.size(), 1u);
  ensure_equals("invalid element", cds_.top(), 123);
}

// test filling stack to the maximum
template<>
template<>
void testObj::test<11>(void)
{
  DivSt ds(2);
  ds.push(11);
  ds.push(22);
  // check size
  ensure_equals("not all elements can be pushed", ds.size(), 2u);
  // check elements
  ensure_equals("invalid element / 1", ds.top(), 22);
  ds.pop();
  ensure_equals("invalid element / 2", ds.top(), 11);
}

// test poping last element - smoke test
template<>
template<>
void testObj::test<12>(void)
{
  ds_.push(1);
  ds_.pop();
  ensure("stack not empty", cds_.empty() );
}

// test overfilling the stack
template<>
template<>
void testObj::test<13>(void)
{
  DivSt ds(2);
  ds.push(10);
  ds.push(20);
  ds.push(30);  // overfilling - last element's out
  ensure_equals("invalid number of elements", ds.size(), 2u);
  ensure_equals("invalid element / 1", ds.top(), 30);
  ds.pop();
  ensure_equals("invalid element / 2", ds.top(), 20);
  ds.pop();
  ensure("stack not empty", ds.empty() );
}

// test stack of maximum size==1
template<>
template<>
void testObj::test<14>(void)
{
  DivSt ds(1);
  ds.push(10);
  ds.push(20);
  ensure_equals("invalid number of elements", ds.size(), 1u);
  ensure_equals("invalid element", ds.top(), 20);
  ds.pop();
  ensure("stack not empty", ds.empty() );
}

// test iterators
template<>
template<>
void testObj::test<15>(void)
{
  ds_.push(10);
  DivSt::iterator it=ds_.begin();
  ensure("no elements?", it!=ds_.end() );
  ensure_equals("invalid value", *it, 10);
  ++it;
  ensure("too many elements", it==ds_.end() );
}

// test writing with iterators
template<>
template<>
void testObj::test<16>(void)
{
  ds_.push(10);
  DivSt::iterator it=ds_.begin();
  ensure("no elements?", it!=ds_.end() );
  *it=42;
  ensure_equals("invalid value", *it, 42);
}

// test const-iterators
template<>
template<>
void testObj::test<17>(void)
{
  ds_.push(10);
  DivSt::const_iterator it=cds_.begin();
  ensure("no elements?", it!=cds_.end() );
  ensure_equals("invalid value", *it, 10);
  ++it;
  ensure("too many elements", it==cds_.end() );
}

// test copy-ctor
template<>
template<>
void testObj::test<18>(void)
{
  ds_.push(42);
  const DivSt tmp(ds_);
  ensure_equals("invalid size", tmp.size(), 1u);
  ensure_equals("invalid element", tmp.top(), 42);
}

// test assignment
template<>
template<>
void testObj::test<19>(void)
{
  ds_.push(42);
  DivSt tmp( ds_.maxSize() );
  tmp=ds_;
  ensure_equals("invalid size", tmp.size(), 1u);
  ensure_equals("invalid element", tmp.top(), 42);
}

// test exception when assigning queues of different sizes
template<>
template<>
void testObj::test<20>(void)
{
  DivSt ds(123);
  try
  {
    ds=ds_; // should throw
    fail("c-tor didn't throw on different maximum size");
  }
  catch(const DivSt::ExceptionDifferentMaximumSizes &)
  {
    // this is expected
  }
}

} // namespace tut
