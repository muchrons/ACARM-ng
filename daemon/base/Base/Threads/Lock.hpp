/*
 * Lock.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_LOCK_HPP_FILE
#define INCLUDE_BASE_THREADS_LOCK_HPP_FILE

#include <boost/thread/locks.hpp>

namespace Base
{
namespace Threads
{

/** \brief mutex-locking class.
 */
typedef boost::lock_guard<Mutex> Lock;

} // namespace Threads
} // namespace Base

#endif
