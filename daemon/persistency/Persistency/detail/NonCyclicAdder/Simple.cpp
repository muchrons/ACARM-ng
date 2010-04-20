/*
 * Simple.cpp
 *
 */
#include "Persistency/detail/NonCyclicAdder.hpp"

namespace Persistency
{
namespace detail
{

class NonCyclicAdder::InternalImplementation
{
}; // class NonCyclicAdder


NonCyclicAdder::NonCyclicAdder(void):
  data_(NULL)   // internal data is NOT needed here
{
}

NonCyclicAdder::~NonCyclicAdder(void)
{
  // NOTE: required for API compatibility
}

void NonCyclicAdder::addChild(GraphNodePtrNN parent, GraphNodePtrNN child)
{
  // TODO
}

} // namespace detail
} // namespace Persistency
