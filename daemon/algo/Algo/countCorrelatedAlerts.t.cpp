/*
 * countCorrelatedAlerts.t.cpp
 *
 */
#include <tut.h>

#include "Algo/countCorrelatedAlerts.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    tree_( makeNewTree1() ),
    nonUniqueTree_( makeNewTree2() )
  {
  }

  GraphNodePtrNN tree_;
  GraphNodePtrNN nonUniqueTree_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/countCorrelatedAlerts");
} // unnamed namespace


namespace tut
{

// test counting leafs when they are all unique
template<>
template<>
void testObj::test<1>(void)
{
  const size_t cnt=countCorrelatedAlerts(tree_);
  ensure_equals("invalid number of elements", cnt, 5);
}

// check leaf traversal
template<>
template<>
void testObj::test<2>(void)
{
  const size_t cnt=countCorrelatedAlerts( makeNewLeaf() );
  ensure_equals("invalid count for leaf", cnt, 1);
}

// check tree with non-unique leafs
template<>
template<>
void testObj::test<3>(void)
{
  const size_t cnt=countCorrelatedAlerts(nonUniqueTree_);
  ensure_equals("invalid number of elements", cnt, 3);
}

} // namespace tut
