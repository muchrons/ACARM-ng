/*
 * ReadTryLock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_READTRYLOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_READTRYLOCK_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/Threads/ReadWriteMutex.hpp"

namespace Base
{
namespace Threads
{

/** \brief mutex read-only try locking class.
 */
class ReadTryLock: private boost::noncopyable
{
public:
  /** \brief try lock given mutex for reading.
   *  \param rwm mutex to be locked.
   */
  explicit ReadTryLock(ReadWriteMutex &rwm):
    rwm_(rwm),
    isLocked_(false)
  {
    tryLock();
  }
  /** \brief unlock mutex, if has been locked.
   */
  ~ReadTryLock(void)
  {
    Lock lock(mutex_);
    if(isLocked_)
      rwm_.unlock_shared();
  }
  /** \brief checks if mutex is locked by this object.
   *  \return true if mutex is locked by this object, false otherwise.
   */
  bool ownsLock(void) const
  {
    Lock lock(mutex_);
    return isLocked_;
  }
  /** \brief try-locking mutex and return result.
   *  \return true if object is locked after this call, false otherwise.
   *  \note if mutex is alrady locked when calling this function it returns
   *        true and does not lock it again.
   */
  bool tryLock(void)
  {
    Lock lock(mutex_);
    if(isLocked_)
      return true;
    isLocked_=rwm_.try_lock_shared();
    return isLocked_;
  }
  /** \brief try-locking mutex for a given amount of time and return result.
   *  \param timeout number of seconds to wait before giving up.
   *  \return true if object is locked after this call, false otherwise.
   *  \note if mutex is alrady locked when calling this function it returns
   *        true and does not lock it again.
   */
  bool tryLock(const unsigned int timeout)
  {
    Lock lock(mutex_);
    if(isLocked_)
      return true;
    isLocked_=rwm_.timed_lock_shared( boost::posix_time::seconds(timeout) );
    return isLocked_;
  }

private:
  ReadWriteMutex &rwm_;
  mutable  Mutex  mutex_;
  bool            isLocked_;
};

} // namespace Threads
} // namespace Base

#endif
