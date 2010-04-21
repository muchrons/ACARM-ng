/*
 * LockOnWrite_InternalImplementation.hpp
 *
 */
#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/ReadWriteMutex.hpp"
#include "Base/Threads/Conditional.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"
#include "Persistency/detail/NonCyclicAdder/WaitingLockData.hpp"
#include "Persistency/detail/NonCyclicAdder/LockingSession.hpp"

namespace Persistency
{
namespace detail
{

// TODO: comment this code

/** \brief helper data structure
 */
struct NonCyclicAdder::InternalImplementation
{
  void addChild(InternalAccessProxy &iap,
                GraphNode           &parent,
                GraphNodePtrNN       child);

private:
  void checkForCycle(LockingSession            &ls,
                     InternalAccessProxy       &iap,
                     GraphNode                 &parent,
                     GraphNode::const_iterator  begin,
                     GraphNode::const_iterator  end,
                     const GraphNode           *checkFor);

  void checkForCycle(LockingSession      &ls,
                     InternalAccessProxy &iap,
                     GraphNode           &parent,
                     GraphNodePtrNN       node,
                     const GraphNode     *checkFor);

  void throwIfDeadlock(InternalAccessProxy &iap,
                       GraphNodePtr         node,
                       const GraphNode     *rootToCheck);

  mutable Base::Threads::ReadWriteMutex mutexRW_;
  WaitingLockData                       wld_;
}; // class InternalImplementation

} // namespace detail
} // namespace Persistency
