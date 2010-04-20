/*
 * LockOnWrite_InternalImplementation.hpp
 *
 */
#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Conditional.hpp"
#include "Persistency/GraphNodePtr.hpp"
#include "Persistency/detail/NonCyclicAdder.hpp"

namespace Persistency
{
namespace detail
{

/** \brief helper object fotr implementeation. not used here.
 */
class NonCyclicAdder::InternalImplementation
{
public:
  InternalImplementation(void);
  ~InternalImplementation(void);

  void setPtr(GraphNodePtr ptr);
  GraphNodePtr getPtr(void) const;
  void signalALl(void) const;
  GraphNodePtr waitIfCannotLock(ReadWriteLock &rwl);

private:
  void setPtrImpl(GraphNodePtr ptr);

  mutable Base::Threads::Mutex       mutex_;
  mutable Base::Threads::Conditional cond_;
  GraphNodePtr                       ptr_;
}; // class InternalImplementation

} // namespace detail
} // namespace Persistency
