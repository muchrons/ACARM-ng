/*
 * TestCallback.t.cpp
 *
 */
#include <tut.h>
#include <boost/type_traits/is_same.hpp>

#include "System/ignore.hpp"
#include "Persistency/Facades/StrAccess/TestCallback.t.hpp"

using namespace std;
using namespace Persistency::Facades::StrAccess;

namespace
{

struct TestClass
{
  TestClass(void):
    base_(callback_)
  {
  }

  TestCallback    callback_;
  ResultCallback &base_;
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
  base_.collectionSize(42);
  ensure_equals("invalid size", callback_.lastSize_, 42);
}

// test callback on value
template<>
template<>
void testObj::test<2>(void)
{
  base_.value("narf");
  ensure_equals("invalid value", callback_.lastValue_, "narf");
}

} // namespace tut
