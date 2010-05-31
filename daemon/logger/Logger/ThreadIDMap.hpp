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

// TODO: test
// TODO: comment

namespace Logger
{

class ThreadIDMap: private boost::noncopyable
{
public:
  ThreadIDMap(void);

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
