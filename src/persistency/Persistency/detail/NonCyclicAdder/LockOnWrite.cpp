/*
 * LockOnWrite.cpp
 *
 */
#include <cassert>

#include "Base/Threads/WriteLock.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"
#include "Persistency/detail/NonCyclicAdder/LockOnWrite_InternalImplementation.hpp"

namespace Persistency
{
namespace detail
{

NonCyclicAdder::NonCyclicAdder(void):
  data_(new InternalImplementation)
{
  assert( data_.get()!=NULL );
}

NonCyclicAdder::~NonCyclicAdder(void)
{
  assert( data_.get()!=NULL );
  // NOTE: this generates valid d-tor for PImpl'ed class.
}

bool NonCyclicAdder::addChildImpl(GraphNode           &parent,
                                  InternalAccessProxy &iap,
                                  GraphNodePtrNN       child)
{
  assert( data_.get()!=NULL );
  // forward call to class that can have independent interface
  return data_->addChild(iap, parent, child);
}

} // namespace detail
} // namespace Persistency
