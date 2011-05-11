/*
 * Thread.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_THREADS_THREAD_HPP_FILE
#define INCLUDE_SYSTEM_THREADS_THREAD_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

#include "System/SharedPtrNotNULL.hpp"

namespace System
{
namespace Threads
{

/** \brief thread wrapper.
 */
class Thread: private boost::noncopyable
{
public:
  /** \brief thread's ID type. */
  typedef boost::thread::id ID;

  /** \brief create thread form a given function object.
   *  \param t user's function object.
   */
  template<typename T>
  explicit Thread(const T &t):
    th_( FuncObj<T>(t) )
  {
  }
  /** \brief interrupt and join thread.
   *
   *  d-tor will end only when thread will be joined.
   */
  ~Thread(void);

  /** \brief joins thread.
   */
  void join(void);
  /** \brief interrput thread.
   */
  void interrupt(void);
  /** \brief gets thread id.
   */
  ID id(void) const;

private:
  template<typename T>
  class FuncObj
  {
  public:
    explicit FuncObj(const T &t):
      t_(t)
    {
    }

    void operator()(void)
    {
      try
      {
        t_();
      }
      catch(const boost::thread_interrupted &)
      {
        // thread interruption requested - exiting thread
      }
      catch(const std::exception &)
      {
        // unhandled error...
      }
      catch(...)
      {
        // unknown error...
      }
      // normal exit
    }

  private:
    T t_;
  }; // class FuncObj

  boost::thread th_;
}; // class Thread


/** \brief shared pointer to thread (usefull in collections). */
typedef System::SharedPtrNotNULL<Thread> ThreadPtrNN;

} // namespace Threads
} // namespace System

#endif
