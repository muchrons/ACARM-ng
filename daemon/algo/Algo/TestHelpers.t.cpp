/*
 * TestHelpers.t.cpp
 *
 */
#include "Algo/TestHelpers.t.hpp"

using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace Algo
{

MetaAlertPtrNN th_makeMetaAlert(void)
{
  return MetaAlertPtrNN( new MetaAlert( MetaAlert::Name("meta-alertX"),
                                        0.1, 0.2,
                                        makeNewReferenceURL(),
                                        Timestamp() ) );
} // th_makeMetaAlert()

GraphNodePtrNN th_makeLeaf(void)
{
  return makeNewLeaf();
} // th_makeLeaf()

GraphNodePtrNN th_makeNode(GraphNodePtrNN child1, GraphNodePtrNN child2)
{
  return makeNewNode(child1, child2);
} // th_makeNode()

GraphNodePtrNN th_makeNode(void)
{
  return makeNewNode();
} // th_makeNode()

GraphNodePtrNN th_makeTree1(void)
{
  return makeNewTree1();
} // th_makeTree1()

GraphNodePtrNN th_makeTree2(void)
{
  return makeNewTree2();
} // th_makeTree2()

} // namespace Algo
