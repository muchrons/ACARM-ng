/*
 * forEachInTree.t.cpp
 *
 */
#include <tut.h>

#include "Algo/forEachInTree.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct FuncObj: private TestHelpers::Persistency::TestStubs
{
  FuncObj(void):
    cnt_(0)
  {
  }
  void operator()(ConstGraphNodePtrNN)
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
  const FuncObj out=forEachInTree(root_, FuncObj() );
  ensure_equals("invalid number of elements", out.cnt_, 9);
}

// check leaf traversal
template<>
template<>
void testObj::test<2>(void)
{
  const FuncObj out=forEachInTree( makeNewLeaf(), FuncObj() );
  ensure_equals("invalid traversal count for leaf", out.cnt_, 1);
}

// check traversal for const object
template<>
template<>
void testObj::test<3>(void)
{
  const ConstGraphNodePtrNN cRoot=root_;
  const FuncObj             out  =forEachInTree( cRoot, FuncObj() );
  ensure_equals("invalid number of elements", out.cnt_, 9);
}

} // namespace tut
