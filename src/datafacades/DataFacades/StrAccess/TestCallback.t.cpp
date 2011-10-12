/*
 * TestCallback.t.cpp
 *
 */
#include <tut.h>

#include "DataFacades/StrAccess/TestCallback.t.hpp"

namespace
{

struct TestClass
{
  Facades::StrAccess::TestCallback callback_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/TestCallback");
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

// test callback on null
template<>
template<>
void testObj::test<3>(void)
{
  callback_.nullOnPath("xyz");
  ensure_equals("invalid value", callback_.lastNullFound_, "xyz");
}

} // namespace tut
