/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_TESTHELPERS_T_HPP
#define INCLUDE_FILTER_TESTHELPERS_T_HPP

#include "TestHelpers/Persistency/TestHelpers.hpp"

// looks bad, but this is header included only in .t.cpp files
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

MetaAlertPtrNN th_makeMetaAlert(void)
{
  return makeNewMetaAlert();
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

} // unnamed namespace

#endif
