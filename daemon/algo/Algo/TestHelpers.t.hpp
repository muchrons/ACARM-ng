/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_ALGO_TESTHELPERS_T_HPP
#define INCLUDE_ALGO_TESTHELPERS_T_HPP

#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

namespace Algo
{

Persistency::MetaAlertPtrNN th_makeMetaAlert(void);

Persistency::GraphNodePtrNN th_makeLeaf(void);

Persistency::GraphNodePtrNN th_makeNode(Persistency::GraphNodePtrNN child1,
                                        Persistency::GraphNodePtrNN child2);

Persistency::GraphNodePtrNN th_makeNode(void);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      leaf5
//                leaf3 leaf4
//
Persistency::GraphNodePtrNN th_makeTree1(void);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      node1
//                leaf3 node1    ...
//                       ...
//
Persistency::GraphNodePtrNN th_makeTree2(void);

} // namespace Alego

#endif
