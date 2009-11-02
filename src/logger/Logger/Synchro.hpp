/*
 * Synchro.hpp
 *
 */
#ifndef INCLUDE_LOGGER_SYNCHRO_HPP_FILE
#define INCLUDE_LOGGER_SYNCHRO_HPP_FILE

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

namespace Logger
{

/** \brief class to be used as a mutex type.
 */
typedef boost::mutex Mutex;

/** \brief mutex-locking class.
 */
typedef boost::lock_guard<Mutex> Lock;

} // namespace Logger

#endif
