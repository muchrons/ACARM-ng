/*
 * forEachUniqueInTree.t.cpp
 *
 */
#include <tut.h>

#include "Algo/forEachUniqueInTree.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

template<typename T>
struct CountNodes: private TestHelpers::Persistency::TestStubs
{
  CountNodes(void):
    cnt_(0)
  {
  }
  void operator()(T)
  {
    ++cnt_;
  }

  int cnt_;
}; // struct CountNodes

struct TestClass
{
};

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
  CountNodes<Persistency::ConstGraphNodePtrNN> cn;
  cn=forEachUniqueInTree( makeNewTree1(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 9);
}

// check leaf traversal
template<>
template<>
void testObj::test<2>(void)
{
  CountNodes<Persistency::ConstGraphNodePtrNN> cn;
  cn=forEachUniqueInTree( makeNewLeaf(), cn );
  ensure_equals("invalid count for leaf", cn.cnt_, 1);
}

// check tree with non-unique leafs and nodes
template<>
template<>
void testObj::test<3>(void)
{
  CountNodes<Persistency::ConstGraphNodePtrNN> cn;
  cn=forEachUniqueInTree( makeNewTree2(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 7);
}

// check const trees
template<>
template<>
void testObj::test<4>(void)
{
  const ConstGraphNodePtrNN                    node=makeNewTree2();
  CountNodes<Persistency::ConstGraphNodePtrNN> cn;
  cn=forEachUniqueInTree(node, cn);
  ensure_equals("invalid number of elements", cn.cnt_, 7);
}

// test counting nodes/leafs via pointer
template<>
template<>
void testObj::test<5>(void)
{
  CountNodes<Persistency::GraphNode*> cn;
  cn=forEachUniqueInTree( makeNewTree1().get(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 9);
}

// test counting nodes/leafs via const pointer
template<>
template<>
void testObj::test<6>(void)
{
  CountNodes<const Persistency::GraphNode*> cn;
  cn=forEachUniqueInTree( makeNewTree1().get(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 9);
}

// check tree with non-const smart pointers
template<>
template<>
void testObj::test<7>(void)
{
  CountNodes<Persistency::GraphNodePtrNN> cn;
  cn=forEachUniqueInTree( makeNewTree2(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 7);
}

} // namespace tut
