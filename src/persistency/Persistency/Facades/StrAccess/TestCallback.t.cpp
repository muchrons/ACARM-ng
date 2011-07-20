/*
 * TestCallback.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/TestCallback.t.hpp"

namespace
{

struct TestClass
{
  TestCallback callback_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/TestCallback");
} // unnamed namespace


namespace tut
{

// test callback on size
template<>
template<>
void testObj::test<1>(void)
{
  callback_.collectionSize(42);
  ensure_equals("invalid size", callback_.lastSize_, 42);
}

// test callback on value
template<>
template<>
void testObj::test<2>(void)
{
  callback_.value("narf");
  ensure_equals("invalid value", callback_.lastValue_, "narf");
}

} // namespace tut
