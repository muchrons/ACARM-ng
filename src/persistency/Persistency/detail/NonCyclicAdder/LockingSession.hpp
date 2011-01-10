/*
 * LockingSession.hpp
 *
 */
#include <set>
#include <vector>
#include <boost/noncopyable.hpp>

#include "Base/Threads/ReadTryLock.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/GraphNodePtr.hpp"

namespace Persistency
{
namespace detail
{

/** \brief collection of nodes and lock that have been already locked (read-only)
 *         by given operation.
 *
 *  having this object makes it possible to avoid checking the same nodes multiple
 *  times and does not require mutexes being locked (if any) to be recurrent.
 */
class LockingSession: private boost::noncopyable
{
public:
  /** \brief helper typedef for mutex type that can be held in side collection. */
  typedef Commons::SharedPtrNotNULL<Base::Threads::ReadTryLock> ReadTryLockPtrNN;

  /** \brief checks if given node is already in collection.
   *  \param node node to check presence of.
   *  \return true if object is in collection, false otherwise.
   */
  bool hasNode(const GraphNode &node) const;
  /** \brief adds new node to collection allong with it's lock.
   *  \param node node to be added.
   *  \param lock lock on this object.
   *  \note lock must be locked when being added.
   */
  void addLockedNode(const GraphNode &node, ReadTryLockPtrNN lock);

private:
  std::set<const GraphNode*>    lockedNodes_;
  std::vector<ReadTryLockPtrNN> locks_;
}; // class LockingSession

} // namespace detail
} // namespace Persistency
