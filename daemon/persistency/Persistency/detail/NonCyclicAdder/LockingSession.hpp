/*
 * LockingSession.hpp
 *
 */
#include <set>
#include <vector>
#include <boost/shared_ptr.hpp>

#include "Base/Threads/ReadTryLock.hpp"
#include "Persistency/GraphNodePtr.hpp"

namespace Persistency
{
namespace detail
{

// TODO: comment this code

/** \brief 
 */
class LockingSession
{
public:
  typedef boost::shared_ptr<Base::Threads::ReadTryLock> ReadTryLockPtr;

  bool hasNode(const GraphNode &node) const;
  void addLockedNode(const GraphNode &node, ReadTryLockPtr lock);

private:
  std::set<const GraphNode*>  lockedNodes_;
  std::vector<ReadTryLockPtr> locks_;
}; // class LockingSession

} // namespace detail
} // namespace Persistency
