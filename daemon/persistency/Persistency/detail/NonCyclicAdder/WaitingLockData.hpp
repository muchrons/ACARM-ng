/*
 * WaitingLockData.hpp
 *
 */
#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/ReadWriteMutex.hpp"
#include "Base/Threads/ReadTryLock.hpp"
#include "Base/Threads/Conditional.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"

namespace Persistency
{
namespace detail
{

/** \brief holder for pointer to blocking item.
 */
class WaitingLockData
{
public:
  WaitingLockData(void);
  ~WaitingLockData(void);

  void setPtr(GraphNodePtr ptr);
  GraphNodePtr getPtr(void) const;
  GraphNodePtr getWhenDifferOrLocked(GraphNodePtr ptr, Base::Threads::ReadTryLock &rtl);
  void signalAll(void);

private:
  void setPtrImpl(GraphNodePtr ptr);

  mutable Base::Threads::Mutex       mutex_;
  mutable Base::Threads::Conditional cond_;
  GraphNodePtr                       ptr_;  // node we're waiting for.
}; // class WaitingLockData

} // namespace detail
} // namespace Persistency
