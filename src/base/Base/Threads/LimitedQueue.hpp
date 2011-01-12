/*
 * LimitedQueue.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_LIMITEDQUEUE_HPP_FILE
#define INCLUDE_BASE_THREADS_LIMITEDQUEUE_HPP_FILE

/* public header */

#include <queue>
#include <boost/static_assert.hpp>
#include <boost/thread/thread.hpp>
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Conditional.hpp"

namespace Base
{
namespace Threads
{
/** \brief FIFO queue of limited size, blocking next addition if full.
 */
template<typename T, size_t N>
class LimitedQueue
{
public:
  /** \brief type held inside the collection. */
  typedef T value_type;

  /** \brief gets top-most element.
   *  \return top-most element from collection.
   */
  value_type top(void) const
  {
    Lock lock(mutex_);
    return topImpl(lock);
  }

  /** \brief gets top most element from collection and removes it from it.
   *  \return top-most element from collection.
   */
  value_type pop(void)
  {
    Lock lock(mutex_);
    const value_type tmp=topImpl(lock);
    q_.pop();
    assert( q_.size()<N );
    notFull_.notify_one();  // notify on size < N
    return tmp;
  }

  /** \brief signals all threads waiting on push()/pop().
   */
  void signalAll(void)
  {
    assert( q_.size()<=N && "collection oversized");
    notFull_.notify_all();
    notEmpty_.notify_all();
  }

  /** \brief adds new element to collection.
   *  \param e element to be added.
   *  \note if collection is full (in terms of N) this call blocks
   */
  void push(const value_type &e)
  {
    Lock lock(mutex_);
    // wait for queue to be processed
    while( q_.size()==N )
    {
      // wait on condition variable is an interruption point itself, thus no explicit check is needed here.
      notFull_.wait(lock);
    }

    // now there is enough space - add element to queue
    assert( q_.size()<N && "collection somehow grew too large" );
    q_.push(e);
    assert( q_.size()<=N && "collection oversized");
    notEmpty_.notify_one(); // notify on size > 0
  }

  /** \brief returns collection's current size.
   *  \return size of collection.
   */
  size_t size(void) const
  {
    Lock lock(mutex_);
    assert( q_.size()<=N && "collection oversized");
    return q_.size();
  }

  /** \brief returns maximum sie of the collection.
   *  \return size given as a template paramter.
   */
  size_t maxSize(void) const
  {
    return N;
  }

private:
  BOOST_STATIC_ASSERT( N>0 );

  value_type topImpl(Lock &lock) const
  {
    // wait for queue to have at least one element
    while( q_.size()==0 )
    {
      boost::this_thread::interruption_point(); //TODO: same as above
      notEmpty_.wait(lock);
    }
    // ok - we have at least one element
    assert( q_.size()>0 );
    return q_.front();
  }

  typedef std::queue<value_type> Queue;

  mutable Mutex       mutex_;
  mutable Conditional notEmpty_;
  mutable Conditional notFull_;
  Queue               q_;
}; // class LimitedQueue

} // namespace Threads
} // namespace Base

#endif
