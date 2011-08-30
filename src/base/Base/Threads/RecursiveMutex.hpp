/*
 * RecursiveMutex.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_RECURSIVEMUTEX_HPP_FILE
#define INCLUDE_BASE_THREADS_RECURSIVEMUTEX_HPP_FILE

/* public header */

#include <boost/thread/recursive_mutex.hpp>

namespace Base
{
namespace Threads
{

/** \brief class to be used as a recursive mutex type. */
typedef boost::recursive_mutex RecursiveMutex;

} // namespace Threads
} // namespace Base

#endif
