/*
 * SignedNode.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/SignedNode.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Core::Types;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/SignedNode");
} // unnamed namespace


namespace tut
{

// test getting node
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::GraphNodePtrNN node=makeNewLeaf();
  SignedNode sn(node, "abc");
  ensure("invalid node returned", node.get()==sn.getNode().get() );
}

// test getting signer's name
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::GraphNodePtrNN node=makeNewLeaf();
  SignedNode sn(node, "abc");
  ensure_equals("invalid name returned", sn.getReporter(), "abc");
}

} // namespace tut
