/*
 * GrowingVectorData.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_DETAIL_GWORINGVECTORDATA_HPP_FILE
#define INCLUDE_BASE_THREADS_DETAIL_GWORINGVECTORDATA_HPP_FILE

/* public header */

#include <vector>

#include "Base/Threads/Mutex.hpp"

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

  /** \brief mutex to guard access to vector. */
  mutable Mutex mutex_;
  /** \brief vector object. */
  V             vec_;
}; // struct GrowingVectorData

} // namespace detail
} // namespace Threads
} // namespace Base

#endif
