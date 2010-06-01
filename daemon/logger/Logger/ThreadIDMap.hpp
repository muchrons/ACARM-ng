/*
 * ThreadIDMap.hpp
 *
 */
#ifndef INCLUDE_LOGGER_THREADIDMAP_HPP_FILE
#define INCLUDE_LOGGER_THREADIDMAP_HPP_FILE

#include <vector>
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "Base/Threads/Mutex.hpp"

namespace Logger
{
/** \brief class mapping getween system-specific thread::id and interers.
 *
 *  each thread getThreadID() is called in gets unique, next in line ID number.
 *  numbering starts with 0.
 */
class ThreadIDMap: private boost::noncopyable
{
public:
  /** \brief create class.
   */
  ThreadIDMap(void);

  /** \brief get current thread number (ID).
   *  \return thread identifier as unsigned integer.
   */
  unsigned int getThreadID(void);

private:
  typedef std::pair<boost::thread::id, unsigned int> Mapping;
  typedef std::vector<Mapping>                       IDMap;

  mutable Base::Threads::Mutex mutex_;
  unsigned int                 nextFreeID_;
  IDMap                        m_;
}; // class ThreadIDMap

} // namespace Logger

#endif
