/*
 * ProcessedSet.t.cpp
 *
 */
#include <tut.h>

#include "Filter/IPBlackList/ProcessedSet.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass
{
  ProcessedSet s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/ProcessedSet");
} // unnamed namespace


namespace tut
{

// check if non-existing entry is reported as such
template<>
template<>
void testObj::test<1>(void)
{
  ensure("non-existing entry is reported as present", s_.isProcessed( makeNewLeaf(), makeNewHost() )==false );
}

// check pruning
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::GraphNodePtrNN n1=makeNewLeaf();
  Persistency::HostPtrNN      h1=makeNewHost();
  s_.markAsProcessed( makeNewLeaf(), makeNewHost() );
  s_.markAsProcessed( n1, h1 );
  s_.markAsProcessed( makeNewLeaf(), makeNewHost() );
  s_.prune();
  ensure("non-NULL element has been pruned", s_.isProcessed(n1, h1) );
}

// test if addition works
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::GraphNodePtrNN n1=makeNewLeaf();
  Persistency::HostPtrNN      h1=makeNewHost();
  Persistency::GraphNodePtrNN n2=makeNewLeaf();
  Persistency::HostPtrNN      h2=makeNewHost();
  s_.markAsProcessed(n1, h1);
  s_.markAsProcessed(n2, h2);
  // check
  ensure("element 1 not present", s_.isProcessed(n1, h1) );
  ensure("element 2 not present", s_.isProcessed(n2, h2) );
}

// test if reporting nodes (not leafs) works
template<>
template<>
void testObj::test<4>(void)
{
  Persistency::GraphNodePtrNN n=makeNewNode();
  Persistency::HostPtrNN      h=makeNewHost();
  s_.markAsProcessed(n, h);
  // check
  ensure("node not present", s_.isProcessed(n, h) );
}

// test if reporting the same element multiple times works
template<>
template<>
void testObj::test<5>(void)
{
  Persistency::GraphNodePtrNN n=makeNewLeaf();
  Persistency::HostPtrNN      h=makeNewHost();
  s_.markAsProcessed(n, h);
  s_.markAsProcessed(n, h);
  // check
  ensure("node not present", s_.isProcessed(n, h) );
}

// test if host is compared via pointer (this should be done this way here, since
// these pointers are taken from nodes any way and comparing pointers is faster
// than values).
template<>
template<>
void testObj::test<6>(void)
{
  Persistency::GraphNodePtrNN n =makeNewLeaf();
  Persistency::HostPtrNN      h1=makeNewHost();
  Persistency::HostPtrNN      h2=makeNewHost();
  s_.markAsProcessed(n, h1);
  s_.markAsProcessed(n, h2);
  // check
  ensure("pair 1 not present", s_.isProcessed(n, h1) );
  ensure("pair 2 not present", s_.isProcessed(n, h2) );
}

} // namespace tut
