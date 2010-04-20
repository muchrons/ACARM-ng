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

namespace Persistency
{
namespace detail
{

/** \brief helper data structure
 */
struct NonCyclicAdder::InternalImplementation
{
  mutable Base::Threads::ReadWriteMutex mutexRW_;
  WaitingLockData                       wld_;
}; // class InternalImplementation

} // namespace detail
} // namespace Persistency
