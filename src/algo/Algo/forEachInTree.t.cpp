/*
 * forEachInTree.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/GraphNode.hpp"
#include "Algo/forEachInTree.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

template<typename T>
struct FuncObj: private TestHelpers::Persistency::TestStubs
{
  FuncObj(void):
    cnt_(0)
  {
  }
  void operator()(T)
  {
    ++cnt_;
  }
  int cnt_;
};


struct TestClass
{
  TestClass(void):
    root_( makeNewTree1() )
  {
  }

  GraphNodePtrNN root_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/forEachInTree");
} // unnamed namespace


namespace tut
{

// check trawersing through all elements
template<>
template<>
void testObj::test<1>(void)
{
  FuncObj<ConstGraphNodePtrNN> fo;
  fo=forEachInTree(root_, fo);
  ensure_equals("invalid number of elements", fo.cnt_, 9);
}

// check leaf traversal
template<>
template<>
void testObj::test<2>(void)
{
  FuncObj<ConstGraphNodePtrNN> fo;
  fo=forEachInTree( makeNewLeaf(), fo );
  ensure_equals("invalid traversal count for leaf", fo.cnt_, 1);
}

// check traversal for const object
template<>
template<>
void testObj::test<3>(void)
{
  const ConstGraphNodePtrNN    cRoot=root_;
  FuncObj<ConstGraphNodePtrNN> fo;
  fo=forEachInTree(cRoot, fo);
  ensure_equals("invalid number of elements", fo.cnt_, 9);
}

// check traversal via pointer to const object
template<>
template<>
void testObj::test<4>(void)
{
  const GraphNode           *cRoot=root_.get();
  FuncObj<const GraphNode*>  fo;
  fo=forEachInTree(cRoot, fo);
  ensure_equals("invalid number of elements", fo.cnt_, 9);
}

// test exceptionon on NULL pointer
template<>
template<>
void testObj::test<5>(void)
{
  FuncObj<const GraphNode*> fo;
  try
  {
    forEachInTree(static_cast<GraphNode*>(NULL), fo);
    fail("NULL pointer accepted");
  }
  catch(const Algo::Exception&)
  {
    // this is expected
  }
}

// check traversal via pointer to non-const object
template<>
template<>
void testObj::test<6>(void)
{
  GraphNode           *cRoot=root_.get();
  FuncObj<GraphNode*>  fo;
  fo=forEachInTree(cRoot, fo);
  ensure_equals("invalid number of elements", fo.cnt_, 9);
}

// check traversal via smart pointer to non-const object
template<>
template<>
void testObj::test<7>(void)
{
  FuncObj<GraphNodePtrNN> fo;
  fo=forEachInTree(root_, fo);
  ensure_equals("invalid number of elements", fo.cnt_, 9);
}

} // namespace tut
