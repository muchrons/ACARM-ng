/*
 * Lock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_LOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_LOCK_HPP_FILE

/* public header */

#include <boost/thread/locks.hpp>

#include "Base/Threads/Mutex.hpp"

namespace Base
{
namespace Threads
{

/** \brief mutex-locking class.
 */
typedef boost::unique_lock<Mutex> Lock;

} // namespace Threads
} // namespace Base

#endif
