/*
 * ThreadJoiner.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_THREADJOINER_HPP_FILE
#define INCLUDE_BASE_THREADS_THREADJOINER_HPP_FILE

/* public header */

#include <boost/thread.hpp>
#include <boost/noncopyable.hpp>
#include <cassert>

#include "System/SignalMasker.hpp"

namespace Base
{
namespace Threads
{

/** \brief wrapper for boost::thread that ensures thread is interrupted
 *         and joined when d-tor is called.
 *
 *  thanks to this class threads are known to be interrupted and joined even
 *  when normal flow (i.e. non-exception one) would not allow this.
 *
 *  \note this class blocks signals handling in thread it represents. use
 *        System::SignalUnmasker to allow signals in given thread.
 */
class ThreadJoiner: private boost::noncopyable
{
public:
  /** \brief create and start thread from any object that would be accepted
   *         by boost::thread itself.
   *  \param t function object to be run in thread.
   */
  template<typename T>
  explicit ThreadJoiner(const T &t):
    th_( (SignalMaskingOperator<T>(t)) )
  {
  }
  /** \brief interrupr and join thread.
   *
   *  interrupt thread and join it. call block until thread
   *  will finish processing.
   */
  ~ThreadJoiner(void)
  {
    assert( boost::this_thread::get_id()!=th_.get_id() );   // sanity check
    th_.interrupt();
    if( th_.joinable() )
      th_.join();
  }

  /** \brief arrow operator to access underlying thread directly.
   *  \return pointer to thread held inside.
   *  \note returned pointer is guaranteed to be non-NULL.
   */
  boost::thread *operator->(void)
  {
    return &th_;
  }

private:

  template<typename T>
  struct SignalMaskingOperator
  {
    /** \brief signal masking operator.
     *  \param t users code.
     */
    explicit SignalMaskingOperator(const T &t):
      t_(t)
    {
    }
    /** \brief operator ()
     */
    void operator()(void)
    {
      // block sinals for this thread
      System::SignalMasker msk;
      // run user's code.
      t_();
    }
    /// \brief user's code.
    T t_;
  };

  boost::thread th_;
}; // class ThreadJoiner

} // namespace Threads
} // namespace Base

#endif
