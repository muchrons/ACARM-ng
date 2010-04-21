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
  NonCyclicAdder &nca=iap.getNonCyclicAdderFromNode(parent);    // short name
  assert( nca.data_.get()==this );  // sanity check - we should be "parent" here

  // open helper scope for internal operations
  {
    // ensure that releaseing parent's lock will be broadcasted and then
    // lock parent for writing (so that others will not be able to use it).
    // NOTE: sequence of creation/destruction of these object is critical!
    //       FIRSTLY pointer must be NULLEd (ResetOnRelease), THEN we can leave
    //       lock without race condition risk THEN others should be informed
    //       about locking oprtunity (i.e. lock is free).
    WaitingLockData::SignalOnRelease signalRelease(nca.data_->wld_);
    WriteLock                        parentLock(nca.data_->mutexRW_);    // exclusive access to root
    WaitingLockData::ResetOnRelease  reset(nca.data_->wld_);

    // look for cycle in structure and throw on error
    LockingSession ls;
    checkForCycle(ls, iap, parent, child, &parent);

    // if there is no cycle, add new child
    iap.addChildToChildrenVector(parent, child);
  } // helper scope - releases all locks and objects

  assert( nca.data_->wld_.getPtr().get()==NULL );
}

void NonCyclicAdder::InternalImplementation::checkForCycle(LockingSession            &ls,
                                                           InternalAccessProxy       &iap,
                                                           GraphNode                 &parent,
                                                           GraphNode::const_iterator  begin,
                                                           GraphNode::const_iterator  end,
                                                           const GraphNode           *checkFor)
{
  assert(checkFor!=NULL);
  // check each element
  for(GraphNode::const_iterator it=begin; it!=end; ++it)
    checkForCycle(ls, iap, parent, *it, checkFor);
}

void NonCyclicAdder::InternalImplementation::checkForCycle(LockingSession      &ls,
                                                           InternalAccessProxy &iap,
                                                           GraphNode           &parent,
                                                           GraphNodePtrNN       node,
                                                           const GraphNode     *checkFor)
{
  assert(checkFor!=NULL);
  // first check if this node has not been already visited
  if( ls.hasNode(*node) )
    return;

  //
  // try-lock(node)
  //
  assert( iap.getNonCyclicAdderFromNode(*node).data_.get()!=NULL );
  ReadWriteMutex                   &mutexRW=iap.getNonCyclicAdderFromNode(*node).data_->mutexRW_;
  LockingSession::ReadTryLockPtrNN  lockPtr( new ReadTryLock(mutexRW) );
  assert( lockPtr.get()!=NULL );
  if( lockPtr->ownsLock()==false )
  {
    // save information on what node we're waiting
    iap.getNonCyclicAdderFromNode(parent).data_->wld_.setPtr(node);
    WaitingLockData &wldNode=iap.getNonCyclicAdderFromNode(*node).data_->wld_;  // short name
    GraphNodePtr     waitFor=wldNode.getPtr();
    do
    {
      // if there is something we wait for, check if it is not a cycle
      throwIfDeadlock(iap, waitFor, checkFor);
      // wait until something changed
      // NOTE: this call tries to lock lockPtr inside
      waitFor=wldNode.getWhenDifferOrLocked(waitFor, *lockPtr);
    }
    while( lockPtr->ownsLock()==false );
  } // if(!owns_lock)

  //
  // save already locked object (node)
  //
  assert( lockPtr->ownsLock() );
  ls.addLockedNode(*node, lockPtr); // save information that we've already been here

  //
  // continue checking children of node
  //
  if( node->isLeaf()==false )
    checkForCycle(ls, iap, parent, node->begin(), node->end(), checkFor);
}

// detect deadlock condition
void NonCyclicAdder::InternalImplementation::throwIfDeadlock(InternalAccessProxy &iap,
                                                             GraphNodePtr         node,
                                                             const GraphNode     *rootToCheck)
{
  assert(rootToCheck!=NULL);
  GraphNodePtr waitFor=node;
  while( waitFor.get()!=NULL )
  {
    // cycle detected?
    if( waitFor.get()==rootToCheck )
    {
      // finding deadlock means that we're back to own node which is a cycle
      throw ExceptionCycleDetected(SYSTEM_SAVE_LOCATION,
                                   rootToCheck->getMetaAlert().getName().get(),
                                   node->getMetaAlert()->getName().get() );
    }
    // proceed with next element
    const WaitingLockData &wldNode=iap.getNonCyclicAdderFromNode(*waitFor).data_->wld_; // short name
    waitFor=wldNode.getPtr();
  } // while(waitFor!=NULL)
}

} // namespace detail
} // namespace Persistency
