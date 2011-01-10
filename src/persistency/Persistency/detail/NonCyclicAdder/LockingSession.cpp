/*
 * LockingSession.cpp
 *
 */
#include <cassert>

#include "Persistency/detail/NonCyclicAdder/LockingSession.hpp"

namespace Persistency
{
namespace detail
{

bool LockingSession::hasNode(const GraphNode &node) const
{
  return lockedNodes_.find(&node)!=lockedNodes_.end();
}

void LockingSession::addLockedNode(const GraphNode &node, ReadTryLockPtrNN lock)
{
  assert( lockedNodes_.size()==locks_.size() );

  // add new node to list of alreayd locked
  assert( hasNode(node)==false );
  lockedNodes_.insert(&node);
  assert( hasNode(node)==true );

  // save lock
  assert( lock.get()!=NULL );
  assert( lock->ownsLock() );
  locks_.push_back(lock);

  // sanity check
  assert( lockedNodes_.size()==locks_.size() );
}

} // namespace detail
} // namespace Persistency
