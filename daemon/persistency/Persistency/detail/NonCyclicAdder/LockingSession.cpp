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

void LockingSession::addLockedNode(const GraphNode &node, ReadTryLockPtr lock)
{
  assert( lockedNodes_.size()==locks_.size() );

  // add new node to list of alreayd locked
  assert( hasNode(node)==false );
  lockedNodes_.insert(&node);
  assert( hasNode(node)==true );
  // save lock
  locks_.push_back(lock);

  assert( lockedNodes_.size()==locks_.size() );
}

} // namespace detail
} // namespace Persistency
