/*
 * SafeValue.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_SAFE_LOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_SAFE_LOCK_HPP_FILE

/* public header */

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/Threads/SequencedLock.hpp"

namespace Base
{
namespace Threads
{

template<typename T>
class SafeValue
{
public:
  SafeValue(void)
  {
  }

  explicit SafeValue(const T &t):
    t_(t)
  {
  }

  SafeValue(const SafeValue<T> &other):
    t_( other.get() )
  {
  }

  const SafeValue<T> &operator=(const SafeValue<T> &other)
  {
    if(this==&other)
      return *this;
    SequencedLock lock(other.m_, m_);
    t_=other.t_;
    return *this;
  }

  T get(void) const
  {
    Lock lock(m_);
    return t_;
  }

  void set(const T &t)
  {
    Lock lock(m_);
    t_=t;
  }

private:
  mutable Mutex m_;
  T             t_;
}; // class SafeValue

} // namespace Threads
} // namespace Base

#endif
