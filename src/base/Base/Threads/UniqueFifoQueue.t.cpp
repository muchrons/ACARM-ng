/*
 * UniqueFifoQueue.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/UniqueFifoQueue.hpp"

using namespace std;
using namespace Base::Threads;

namespace
{
typedef UniqueFifoQueue<long> UFQ;

struct TestClass
{
  UFQ q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/UniqueFifoQueue");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  UFQ q;
}

// test push/pop
template<>
template<>
void testObj::test<2>(void)
{
  q_.push(42);
  const long tmp=q_.pop();
  ensure_equals("invalid value", tmp, 42);
}

// test adding unique entries
template<>
template<>
void testObj::test<3>(void)
{
  q_.push(3);
  q_.push(1);
  q_.push(5);
  ensure_equals("invalid size", q_.size(), 3);
}

// test adding non-unique elements
template<>
template<>
void testObj::test<4>(void)
{
  q_.push(3);
  q_.push(1);
  q_.push(5);
  ensure_equals("initial addition failed", q_.size(), 3);
  q_.push(1);   // repeated entry
  ensure_equals("non-unique entry has been added", q_.size(), 3);
}

} // namespace tut
