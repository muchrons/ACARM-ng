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
  ReadTryLock(ReadWriteMutex &rwm):
    rwm_(rwm),
    isLocked_(false)
  {
    tryLock();
  }

  ~ReadTryLock(void)
  {
    Lock lock(mutex_);
    if(isLocked_)
      rwm_.unlock_shared();
  }

  bool ownsLock(void) const
  {
    Lock lock(mutex_);
    return isLocked_;
  }

  bool tryLock(void)
  {
    Lock lock(mutex_);
    if(isLocked_)
      return true;
    isLocked_=rwm_.try_lock_shared();
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
