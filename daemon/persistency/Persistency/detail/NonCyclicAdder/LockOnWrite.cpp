/*
 * LockOnWrite.cpp
 *
 */
#include "Persistency/detail/NonCyclicAdder.hpp"

namespace Persistency
{
namespace detail
{

class NonCyclicAdder::InternalImplementation
{
public:
  // TODO
}; // class NonCyclicAdder


NonCyclicAdder::NonCyclicAdder(void):
  data_(new InternalImplementation)
{
}

NonCyclicAdder::~NonCyclicAdder(void)
{
  // NOTE: this generates valid d-tor for PImpl'ed class.
}

void NonCyclicAdder::addChild(GraphNodePtrNN parent, GraphNodePtrNN child)
{
  // TODO
}

} // namespace detail
} // namespace Persistency
