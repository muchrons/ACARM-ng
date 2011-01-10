/*
 * ReadLock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_READLOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_READLOCK_HPP_FILE

/* public header */

#include <boost/thread/locks.hpp>

#include "Base/Threads/ReadWriteMutex.hpp"

namespace Base
{
namespace Threads
{

/** \brief mutex read-locking class.
 */
typedef boost::shared_lock<ReadWriteMutex> ReadLock;

} // namespace Threads
} // namespace Base

#endif
