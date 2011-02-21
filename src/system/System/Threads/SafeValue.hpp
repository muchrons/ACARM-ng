/*
 * SafeValue.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_THREADS_SAFEVALUE_HPP_FILE
#define INCLUDE_SYSTEM_THREADS_SAFEVALUE_HPP_FILE

/* public header */

#include <boost/thread/mutex.hpp>
#include <boost/thread/locks.hpp>

#include "System/Threads/MultiLock.hpp"

namespace System
{
namespace Threads
{

/** \brief wrapper for simple data types that makes them thread-safe.
 */
template<typename T>
class SafeValue
{
private:
  typedef boost::unique_lock<boost::mutex> Lock;
  typedef MultiLock<boost::mutex, Lock>    BMLock;

public:
  /** \brief call default c-tor.
   */
  SafeValue(void)
  {
  }
  /** \brief create from a value.
   *  \param t value to be copyied
   */
  explicit SafeValue(const T &t):
    t_(t)
  {
  }
  /** \brief copy c-tor.
   *  \param other element to copy from.
   */
  SafeValue(const SafeValue &other):
    t_( other.get() )
  {
  }

  /** \brief assignment operator.
   *  \param other object to assign from.
   *  \return const-reference to this.
   */
  const SafeValue &operator=(const SafeValue &other)
  {
    if(&other==this)
      return *this;
    const BMLock lock(mutex_, other.mutex_);
    t_=other.t_;
    return *this;
  }

  /** \brief get object's value.
   */
  T get(void) const
  {
    Lock lock(mutex_);
    return t_;
  }

  /** \brief swap two object's contents.
   *  \param other object ot swap content with.
   */
  void swap(SafeValue &other)
  {
    const BMLock lock(mutex_, other.mutex_);
    t_.swap(other.t_);
  }

private:
  mutable boost::mutex mutex_;
  T                    t_;
}; // class SafeValue

} // namespac Threads
} // namespac System

#endif
