/*
 * LockOnWrite_InternalImplementation.cpp
 *
 */
#include <cassert>

#include "Base/Threads/WriteLock.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/InternalAccessProxy.hpp"
#include "Persistency/detail/NonCyclicAdder/LockOnWrite_InternalImplementation.hpp"

using namespace Base::Threads;


namespace Persistency
{
namespace detail
{

void NonCyclicAdder::InternalImplementation::addChild(InternalAccessProxy &iap,
                                                      GraphNode           &parent,
                                                      GraphNodePtrNN       child)
{
  // ensure that releaseing parent's lock will be broadcasted and then
  // lock parent for writing (so that others will not be able to use it).
  // NOTE: sequence of creation/destruction of these object is critical!
  //       FIRSTLY lock must be released and THEN others should be informed
  //       about locking oprtunity.
  NonCyclicAdder                   &nca=iap.getNonCyclicAdderFromNode(parent);
  assert(nca.data_.get()==this);    // sanity check - we should be "parent" here
  WaitingLockData::SignalOnRelease  parentReleased(nca.data_->wld_);
  WriteLock                         parentLock(nca.data_->mutexRW_);    // exclusive access to this node

  // look for cycle in structure and throw on error
  // TODO: set<*> and vector<ReadTryLock> are needed here (wrapped)
  checkForCycle(iap, child, &parent);

  // if there is no cycle, add new child
  iap.addChildToChildrenVector(parent, child);
}

void NonCyclicAdder::InternalImplementation::checkForCycle(InternalAccessProxy &iap,
                                                           GraphNodePtrNN       node,
                                                           const GraphNode     *checkFor)
{
  assert(checkFor!=NULL);
  checkForCycle(iap, *node, checkFor);
}

void NonCyclicAdder::InternalImplementation::checkForCycle(InternalAccessProxy       &iap,
                                                           GraphNode::const_iterator  begin,
                                                           GraphNode::const_iterator  end,
                                                           const GraphNode           *checkFor)
{
  assert(checkFor!=NULL);
  // check each element
  for(GraphNode::const_iterator it=begin; it!=end; ++it)
    checkForCycle(iap, *it, checkFor);
}

void NonCyclicAdder::InternalImplementation::checkForCycle(InternalAccessProxy &iap,
                                                           GraphNode           &node,
                                                           const GraphNode     *checkFor)
{
  assert(checkFor!=NULL);
  // TODO: try-lock

  // TODO: work on already locked object
  if( node.isLeaf() )
  {
  }
}

} // namespace detail
} // namespace Persistency
