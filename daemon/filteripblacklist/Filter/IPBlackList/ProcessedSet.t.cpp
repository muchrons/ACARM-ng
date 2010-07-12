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
  ensure("non-existing entry is reported as present", s_.isProcessed( makeNewLeaf() )==false );
}

// check pruning
template<>
template<>
void testObj::test<2>(void)
{
  Persistency::GraphNodePtrNN n1=makeNewLeaf();
  s_.markAsProcessed( makeNewLeaf() );
  s_.markAsProcessed( n1 );
  s_.markAsProcessed( makeNewLeaf() );
  s_.prune();
  ensure("non-NULL element has been pruned", s_.isProcessed(n1) );
}

// test if addition works
template<>
template<>
void testObj::test<3>(void)
{
  Persistency::GraphNodePtrNN n1=makeNewLeaf();
  Persistency::GraphNodePtrNN n2=makeNewLeaf();
  s_.markAsProcessed(n1);
  s_.markAsProcessed(n2);
  // check
  ensure("element 1 not present", s_.isProcessed(n1) );
  ensure("element 2 not present", s_.isProcessed(n2) );
}

// test if reporting nodes (not leafs) works
template<>
template<>
void testObj::test<4>(void)
{
  Persistency::GraphNodePtrNN n=makeNewNode();
  s_.markAsProcessed(n);
  // check
  ensure("node not present", s_.isProcessed(n) );
}

// test if reporting the same element multiple times works
template<>
template<>
void testObj::test<5>(void)
{
  Persistency::GraphNodePtrNN n=makeNewLeaf();
  s_.markAsProcessed(n);
  s_.markAsProcessed(n);
  // check
  ensure("node not present", s_.isProcessed(n) );
}

} // namespace tut
