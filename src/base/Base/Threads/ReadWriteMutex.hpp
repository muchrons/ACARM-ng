/*
 * ReadWriteMutex.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_READWRITEMUTEX_HPP_FILE
#define INCLUDE_BASE_THREADS_READWRITEMUTEX_HPP_FILE

/* public header */

#include <boost/thread/shared_mutex.hpp>

namespace Base
{
namespace Threads
{

/** \brief class to be used as a multiple-readers single-writer mutex type.
 */
typedef boost::shared_mutex ReadWriteMutex;

} // namespace Threads
} // namespace Base

#endif
