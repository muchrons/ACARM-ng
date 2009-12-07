/*
 * Lock.hpp
 *
 */
#ifndef INCLUDE_COMMONS_THREADS_LOCK_HPP_FILE
#define INCLUDE_COMMONS_THREADS_LOCK_HPP_FILE

#include <boost/thread/locks.hpp>

namespace Commons
{
namespace Threads
{

/** \brief mutex-locking class.
 */
typedef boost::lock_guard<Mutex> Lock;

} // namespace Threads
} // namespace Commons

#endif
