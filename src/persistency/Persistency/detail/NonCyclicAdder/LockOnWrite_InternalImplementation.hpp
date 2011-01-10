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

/** \brief helper data structure's implemenation
 */
struct NonCyclicAdder::InternalImplementation
{
  /** \brief add new child ot given parent.
   *  \param iap    helper object to get acces to some of GraphNode's internals
   *  \param parent parent node to add child to.
   *  \param child  child to be added to parent.
   *  \return return false if child's not unique, false otherwise.
   */
  bool addChild(InternalAccessProxy &iap,
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

  mutable Base::Threads::ReadWriteMutex mutexRW_;   // multiple-readers/single-writer mutex for GraphNode
  WaitingLockData                       wld_;       // helper class holding extra information
}; // class InternalImplementation

} // namespace detail
} // namespace Persistency
