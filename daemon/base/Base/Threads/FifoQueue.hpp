/*
 * FifoQueue.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_FIFOQUEUE_HPP_FILE
#define INCLUDE_BASE_THREADS_FIFOQUEUE_HPP_FILE

/* public header */

#include <deque>
#include <cassert>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/Threads/Conditional.hpp"

namespace Base
{
namespace Threads
{

/** \brief first-in-first-out queue - thread safe implementation.
 *  \note T many not throw uppon copying, or one will loose enqueued elements.
 */
template<typename T>
class FifoQueue
{
public:
  /** \brief gets first element out.
   *  \return element from the begin of the queue (oldest).
   *  \note this call returns element by value AND removes it from the
   *        queue. this means that copy constructor of type T may NOT
   *        throw - otherwise element might be lost.
   */
  T pop(void)
  {
    // wait for data, if not present
    Lock lock(mutex_);
    while( q_.size()<1 )
      cond_.wait(lock);

    // take one element and return it
    assert( q_.size()>0 );
    const T tmp=q_.front();
    q_.pop_front();
    return tmp;
  }

  /** \brief adds new element to queue.
   *  \param t element to be added.
   */
  void push(const T &t)
  {
    {
      Lock lock(mutex_);
      q_.push_back(t);      // add element to queue
    }
    cond_.notify_one();     // signal presence of new entry
  }

  /** \brief returns size of queue.
   *  \return queue size in terms of elements count.
   *  \note since this class is used with threads, just after
   *        this call exits, information provided by it may already by
   *        outdated. this call has rather log/information status than any
   *        precise information.
   */
  size_t size(void) const
  {
    Lock lock(mutex_);
    return q_.size();
  }

private:
  typedef std::deque<T> Queue;

  mutable Mutex mutex_;
  Conditional   cond_;
  Queue         q_;
}; // class FifoQueue

} // namespace Threads
} // namespace Base

#endif
