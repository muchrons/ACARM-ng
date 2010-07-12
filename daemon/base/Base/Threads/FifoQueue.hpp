/*
 * FifoQueue.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_FIFOQUEUE_HPP_FILE
#define INCLUDE_BASE_THREADS_FIFOQUEUE_HPP_FILE

/* public header */

#include <deque>
#include <boost/thread.hpp>
#include <cassert>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/Threads/Conditional.hpp"
#include "Base/Threads/FifoAcceptAllPolicy.hpp"

namespace Base
{
namespace Threads
{

/** \brief first-in-first-out queue - thread safe implementation.
 *  \note T many not throw uppon copying, or one will loose enqueued elements.
 */
template<typename T, typename AddPolicy=FifoAcceptAllPolicy>
class FifoQueue
{
public:
  /** \brief type of values held inside. */
  typedef T value_type;

  /** \brief gets first element out and removes it from queue.
   *  \return element from the begin of the queue (oldest).
   *
   *  \note this call returns element by value AND removes it from the
   *        queue. this means that copy constructor of type T may NOT
   *        throw - otherwise element might be lost. if this is not the
   *        case use top() call instead.
   *
   *  \note interruption point works ONLY if it if run in a separate thread,
   *        so pop() cannot be stopped, when run from main thread (only by
   *        adding new element).
   */
  T pop(void)
  {
    // wait for data, if not present
    Lock lock(mutex_);
    const T tmp=popImpl(lock);
    q_.pop_front();
    return tmp;
  }

  /** \brief gets first element (does not remove it from queue).
   *  \return element from the begin of the queue (oldest).
   *
   *  \note interruption point works ONLY if it if run in a separate thread,
   *        so pop() cannot be stopped, when run from main thread (only by
   *        adding new element).
   */
  T top(void)
  {
    // wait for data, if not present
    Lock lock(mutex_);
    return popImpl(lock);
  }

  /** \brief signals all threads waiting on pop().
   */
  void signalAll(void)
  {
    cond_.notify_all();
  }

  /** \brief adds new element to queue.
   *  \param t element to be added.
   */
  void push(const T &t)
  {
    {
      const AddPolicy ap=AddPolicy();
      const Lock      lock(mutex_);
      if( !ap(q_, t) )      // element should not be added?
        return;
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
  const T &popImpl(Lock &lock) const
  {
    // wait for data, if not present
    while( q_.size()<1 )
    {
      boost::this_thread::interruption_point();
      cond_.wait(lock);
    }

    // take one element and return it
    assert( q_.size()>0 );
    return q_.front();
  }

  // NOTE: std::deque is used here instead of std::queue since deque has begin/end
  //       iterators required in some circumstances.
  typedef std::deque<T> Queue;

  mutable Mutex       mutex_;
  mutable Conditional cond_;
  Queue               q_;
}; // class FifoQueue

} // namespace Threads
} // namespace Base

#endif
