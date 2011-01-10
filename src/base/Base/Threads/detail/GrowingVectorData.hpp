/*
 * GrowingVectorData.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_DETAIL_GWORINGVECTORDATA_HPP_FILE
#define INCLUDE_BASE_THREADS_DETAIL_GWORINGVECTORDATA_HPP_FILE

/* public header */

#include <vector>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/Threads/SequencedLock.hpp"

namespace Base
{
namespace Threads
{
namespace detail
{

/** \brief helper object that keeps all GrowingVector's data inside.
 */
template<typename T>
struct GrowingVectorData
{
  /** \brief short name for type. */
  typedef std::vector<T> V;

  /** \brief allow default c-tor.
   */
  GrowingVectorData(void)
  {
  }

  /** \brief allows copying.
   *  \param other object to copy from.
   */
  GrowingVectorData(const GrowingVectorData<T> &other)
  {
    Lock lock(other.mutex_);
    vec_=other.vec_;
  }

  /** \brief allow assignment.
   *  \param other object to assign from.
   *  \return const-reference to this.
   */
  const GrowingVectorData<T> &operator=(const GrowingVectorData<T> &other)
  {
    if(&other!=this)    // copy only on non-self assignment
    {
      SequencedLock slock(mutex_, other.mutex_);
      V tmp(other.vec_);
      vec_.swap(tmp);
    }
    return *this;
  }

  /** \brief mutex to guard access to vector. */
  mutable Mutex mutex_;
  /** \brief vector object. */
  V             vec_;
}; // struct GrowingVectorData

} // namespace detail
} // namespace Threads
} // namespace Base

#endif
