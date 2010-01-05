/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_ALGO_TESTHELPERS_T_HPP
#define INCLUDE_ALGO_TESTHELPERS_T_HPP

#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/Stubs/TestHelpers.hpp"

// looks bad, but this is header included only in .t.cpp files
using namespace Persistency;

namespace
{

MetaAlertPtrNN th_makeMetaAlert(void)
{
  return MetaAlertPtrNN( new MetaAlert( MetaAlert::Name("meta-alertX"),
                                        0.1, 0.2,
                                        Stubs::makeNewReferenceURL(),
                                        Timestamp() ) );
} // th_makeMetaAlert()

GraphNodePtrNN th_makeLeaf(void)
{
  return Stubs::makeNewLeaf();
} // th_makeLeaf()

GraphNodePtrNN th_makeNode(GraphNodePtrNN child1, GraphNodePtrNN child2)
{
  return Stubs::makeNewNode(child1, child2);
} // th_makeNode()

GraphNodePtrNN th_makeNode(void)
{
  return Stubs::makeNewNode();
} // th_makeNode()

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      leaf5
//                leaf3 leaf4
//
GraphNodePtrNN th_makeTree1(void)
{
  return Stubs::makeNewTree1();
} // th_makeTree1()

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      node1
//                leaf3 node1    ...
//                       ...
//
GraphNodePtrNN th_makeTree2(void)
{
  return Stubs::makeNewTree2();
} // th_makeTree2()

} // unnamed namespace

#endif
