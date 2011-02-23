/*
 * MultiLock.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_THREADS_MULTILOCK_HPP_FILE
#define INCLUDE_SYSTEM_THREADS_MULTILOCK_HPP_FILE

/* public header */

#include <algorithm>

namespace System
{
namespace Threads
{

/** \brief meta-lock, locking given lokable object in predefined sequence.
 *
 *  when locking always takes place in the same order it is formally proven not to cause
 *  deadlocks. this class perofmrs locking of given mutexes always in the same order.
 */
template<typename TMutex, typename TLock>
class MultiLock
{
public:
  /** \brief lock given mutexes in proper order.
   *  \param m1 mutex to be locked.
   *  \param m2 mutex to be locked.
   */
  MultiLock(TMutex &m1, TMutex &m2):
    lock1_( *std::min(&m1, &m2) ),
    lock2_( *std::max(&m1, &m2) )
  {
  }

private:
  TLock lock1_;
  TLock lock2_;
}; // class MultiLock

} // namespac Threads
} // namespac System

#endif
