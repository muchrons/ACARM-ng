/*
 * ThreadJoiner.hpp
 *
 */
#ifndef INCLUDE_BASE_THREADS_THREADJOINER_HPP_FILE
#define INCLUDE_BASE_THREADS_THREADJOINER_HPP_FILE

/* public header */

#include <boost/thread.hpp>
#include <boost/scoped_ptr.hpp>
#include <cassert>

namespace Base
{
namespace Threads
{

/** \brief wrapper for boost::thread that ensures thread is interrupted
 *         and joined when d-tor is called.
 *
 *  thanks to this class threads are known to be nterrupted and joined even
 *  when normal flow (i.e. non-exception one) would not allow this.
 */
class ThreadJoiner
{
public:
  /** \brief create and start thread from any object that would be accepted
   *         by boost::thread itself.
   *  \param t function object to be run in thread.
   */
  template<typename T>
  explicit ThreadJoiner(const T &t):
    holder_( new Holder<T>(t) ),
    th_( Operate<T>( &static_cast< Holder<T>* >(holder_.get())->t_ ) )
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
  struct HolderBase
  {
    virtual ~HolderBase(void)
    {
    }
  }; // 

  template<typename T>
  struct Holder: public HolderBase
  {
    explicit Holder(const T &t):
      t_(t)
    {
    }

    T t_;
  }; // 

  template<typename T>
  struct Operate
  {
    explicit Operate(T *t):
      t_(t)
    {
    }

    void operator()(void)
    {
      (*t_)();
    }

    T *t_;
  }; // 

  boost::scoped_ptr<HolderBase> holder_;
  boost::thread                 th_;
}; // class ThreadJoiner

} // namespace Threads
} // namespace Base

#endif
