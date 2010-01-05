/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_FILTER_TESTHELPERS_T_HPP
#define INCLUDE_FILTER_TESTHELPERS_T_HPP

#include "Persistency/Stubs/TestHelpers.hpp"

// looks bad, but this is header included only in .t.cpp files
using namespace Persistency;

namespace
{

MetaAlertPtrNN th_makeMetaAlert(void)
{
  return Stubs::makeNewMetaAlert();
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

} // unnamed namespace

#endif
