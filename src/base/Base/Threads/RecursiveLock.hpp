/*
 * RecursiveLock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_RECURSIVELOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_RECURSIVELOCK_HPP_FILE

/* public header */

#include <boost/thread/locks.hpp>

#include "Base/Threads/RecursiveMutex.hpp"

namespace Base
{
namespace Threads
{

/** \brief mutex-locking class.
 */
typedef boost::unique_lock<RecursiveMutex> RecursiveLock;

} // namespace Threads
} // namespace Base

#endif
