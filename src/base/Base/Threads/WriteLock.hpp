/*
 * WriteLock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_WRITELOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_WRITELOCK_HPP_FILE

/* public header */

#include <boost/thread/locks.hpp>

#include "Base/Threads/ReadWriteMutex.hpp"

namespace Base
{
namespace Threads
{

/** \brief mutex write-locking class.
 */
typedef boost::unique_lock<ReadWriteMutex> WriteLock;

} // namespace Threads
} // namespace Base

#endif
