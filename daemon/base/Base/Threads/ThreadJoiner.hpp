/*
 * ThreadJoiner.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_THREADJOINER_HPP_FILE
#define INCLUDE_BASE_THREADS_THREADJOINER_HPP_FILE

/* public header */

#include <boost/thread.hpp>

namespace Base
{
namespace Threads
{

class ThreadJoiner
{
public:
  template<typename T>
  explicit ThreadJoiner(const T &t):
    th_(t)
  {
  }

  ~ThreadJoiner(void)
  {
    th_.interrupt();
    th_.join();
  }

  boost::thread *operator->(void)
  {
    return &th_;
  }

private:
  boost::thread th_;
}; // class ThreadJoiner

} // namespace Threads
} // namespace Base

#endif
