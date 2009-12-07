/*
 * Mutex.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_MUTEX_HPP_FILE
#define INCLUDE_BASE_THREADS_MUTEX_HPP_FILE

/* public header */

#include <boost/thread/mutex.hpp>

namespace Base
{
namespace Threads
{

/** \brief class to be used as a mutex type.
 */
typedef boost::mutex Mutex;

} // namespace Threads
} // namespace Base

#endif
