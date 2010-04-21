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

/** \brief thread-safe value wrapper.
 *  \param T type to hold inside.
 */
template<typename T>
class SafeValue
{
public:
  /** \brief construct with default c-tor.
   */
  SafeValue(void)
  {
  }
  /** \brief create from given value.
   *  \param t alue to be held inside.
   */
  explicit SafeValue(const T &t):
    t_(t)
  {
  }
  /** \brief copy data form given instance.
   *  \param other object to copy data from.
   */
  SafeValue(const SafeValue<T> &other):
    t_( other.get() )
  {
  }
  /** \brief copy value from ginve object.
   *  \param other object to copy value from.
   *  \return const reference to this.
   */
  const SafeValue<T> &operator=(const SafeValue<T> &other)
  {
    if(this==&other)
      return *this;
    SequencedLock lock(other.m_, m_);
    t_=other.t_;
    return *this;
  }
  /** \brief give access to value held inside.
   *  \return value held inside.
   */
  T get(void) const
  {
    Lock lock(m_);
    return t_;
  }
  /** \brief set new value.
   *  \param t value to be set.
   */
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
