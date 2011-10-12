/*
 * collectionSize.t.cpp
 *
 */
#include <tut.h>
#include <list>

#include "DataFacades/StrAccess/collectionSize.hpp"

using namespace Facades::StrAccess;

namespace
{

struct TestClass
{
  void add(const size_t count)
  {
    for(size_t i=0; i<count; ++i)
      c_.push_back(42+i);
  }

  std::list<int> c_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("DataFacades/StrAccess/collectionSize");
} // unnamed namespace


namespace tut
{

// test size of an empty range
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid size of the empty collection", collectionSize(c_.begin(), c_.end()), 0);
}

// test size of an empty collection
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid size of the empty collection", collectionSize(c_.begin(), c_.begin()), 0);
}

// test size of a randomly filled collection
template<>
template<>
void testObj::test<3>(void)
{
  add(13);
  ensure_equals("invalid size of collection", collectionSize(c_.begin(), c_.end()), 13);
}

} // namespace tut
