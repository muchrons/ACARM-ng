/*
 * forEachUniqueLeaf.t.cpp
 *
 */
#include <tut.h>

#include "Algo/forEachUniqueLeaf.hpp"
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

factory tf("Algo/forEachUniqueLeaf");
} // unnamed namespace


namespace tut
{

// test counting leafs when they are all unique
template<>
template<>
void testObj::test<1>(void)
{
  CountNodes<ConstGraphNodePtrNN> cn;
  cn=forEachUniqueLeaf( makeNewTree1(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 5);
}

// check leaf traversal
template<>
template<>
void testObj::test<2>(void)
{
  CountNodes<ConstGraphNodePtrNN> cn;
  cn=forEachUniqueLeaf( makeNewLeaf(), cn );
  ensure_equals("invalid count for leaf", cn.cnt_, 1);
}

// check tree with non-unique leafs and nodes
template<>
template<>
void testObj::test<3>(void)
{
  CountNodes<ConstGraphNodePtrNN> cn;
  cn=forEachUniqueLeaf( makeNewTree2(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 3);
}

// check processing for consts
template<>
template<>
void testObj::test<4>(void)
{
  const ConstGraphNodePtrNN       root=makeNewTree2();
  CountNodes<ConstGraphNodePtrNN> cn;
  cn=forEachUniqueLeaf(root, cn);
  ensure_equals("invalid number of elements", cn.cnt_, 3);
}

// check tree via non-const smart pointer
template<>
template<>
void testObj::test<5>(void)
{
  CountNodes<GraphNodePtrNN> cn;
  cn=forEachUniqueLeaf( makeNewTree2(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 3);
}

// check tree via non-const pointer
template<>
template<>
void testObj::test<6>(void)
{
  CountNodes<GraphNode*> cn;
  cn=forEachUniqueLeaf( makeNewTree2().get(), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 3);
}

// check tree via const pointer
template<>
template<>
void testObj::test<7>(void)
{
  CountNodes<const GraphNode*> cn;
  cn=forEachUniqueLeaf( static_cast<const GraphNode*>(makeNewTree2().get()), cn );
  ensure_equals("invalid number of elements", cn.cnt_, 3);
}

// test exceptionon on NULL pointer
template<>
template<>
void testObj::test<8>(void)
{
  CountNodes<const GraphNode*> cn;
  try
  {
    forEachUniqueInTree(static_cast<GraphNode*>(NULL), cn);
    fail("NULL pointer accepted");
  }
  catch(const Algo::Exception&)
  {
    // this is expected
  }
}

} // namespace tut
