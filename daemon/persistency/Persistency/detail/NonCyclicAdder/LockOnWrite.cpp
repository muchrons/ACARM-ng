/*
 * LockOnWrite.cpp
 *
 */
#include <cassert>

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

void NonCyclicAdder::addChildImpl(const GraphNode *parent, GraphNodePtrNN child)
{
  assert( data_.get()!=NULL );
  assert( parent!=NULL );
  // TODO

  // if there is no cycle, add new child
  addChildToChildrenVector(child);
}

} // namespace detail
} // namespace Persistency
