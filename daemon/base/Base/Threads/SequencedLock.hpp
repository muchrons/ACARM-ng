/*
 * SequencedLock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_SEQUENCEDLOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_SEQUENCEDLOCK_HPP_FILE

/* public header */

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"

namespace Base
{
namespace Threads
{

/** \brief locks mutexes always in the same order.
 *
 * class solves problem of locking the same mutexes from different threads
 * in different order, which leads to deadlock. this class ensures mutexes
 * will be locked always in the same order, though order is not known
 * a'priori. this that following calls will lock in the same order:
 *
 * <code>
 *  Mutex m1;
 *  Mutex m2;
 *  {
 *    SequencedLock lock(m1, m2);
 *  }
 *  {
 *    SequencedLock lock(m2, m1);
 *  }
 *  </code>
 */
class SequencedLock
{
public:
  /** \brief locks mutexes in proper order.
   *  \param m1 mutex 1 to be locked.
   *  \param m2 mutex 2 to be locked.
   */
  SequencedLock(Mutex &m1, Mutex &m2);

private:
  Lock l1_;
  Lock l2_;
}; // class SequencedLock

} // namespace Threads
} // namespace Base

#endif
