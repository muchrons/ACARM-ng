/*
 * LockOnWrite_InternalImplementation.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"

namespace Persistency
{
namespace detail
{

InternalImplementation(void)
{
}

~InternalImplementation(void)
{
}

void setPtr(GraphNodePtr ptr)
{
}

GraphNodePtr getPtr(void) const
{
}

void signalALl(void) const
{
}

GraphNodePtr waitIfCannotLock(ReadWriteLock &rwl)
{
}

} // namespace detail
} // namespace Persistency
