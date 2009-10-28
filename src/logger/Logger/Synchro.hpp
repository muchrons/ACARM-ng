/*
 * Synchro.hpp
 *
 */
#ifndef INCLUDE_LOGGER_SYNCHRO_HPP_FILE
#define INCLUDE_LOGGER_SYNCHRO_HPP_FILE

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>

// TODO: comment

namespace Logger
{

typedef boost::mutex Mutex;

typedef boost::lock_guard<Mutex> Lock;

} // namespace Logger

#endif
