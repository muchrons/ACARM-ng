/*
 * forEachUniqueInTree.t.cpp
 *
 */
#include <tut.h>

#include "Algo/forEachUniqueInTree.hpp"
#include "Algo/TestHelpers.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;

namespace
{

struct CountNodes: private TestHelpers::Persistency::TestStubs
{
  CountNodes(void):
    cnt_(0)
  {
  }
  void operator()(Persistency::GraphNodePtrNN)
  {
    ++cnt_;
  }

  int cnt_;
}; // struct CountNodes

struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/forEachUniqueInTree");
} // unnamed namespace


namespace tut
{

// test counting nodes/leafs when they are all unique
template<>
template<>
void testObj::test<1>(void)
{
  CountNodes cn=forEachUniqueInTree( th_makeTree1(), CountNodes() );
  ensure_equals("invalid number of elements", cn.cnt_, 9);
}

// check leaf traversal
template<>
template<>
void testObj::test<2>(void)
{
  CountNodes cn=forEachUniqueInTree( th_makeLeaf(), CountNodes() );
  ensure_equals("invalid count for leaf", cn.cnt_, 1);
}

// check tree with non-unique leafs and nodes
template<>
template<>
void testObj::test<3>(void)
{
  CountNodes cn=forEachUniqueInTree( th_makeTree2(), CountNodes() );
  ensure_equals("invalid number of elements", cn.cnt_, 7);
}

} // namespace tut
