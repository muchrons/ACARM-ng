/*
 * Thread.hpp
 *
 */
#ifndef INCLUDE_COMMONS_THREADS_THREAD_HPP_FILE
#define INCLUDE_COMMONS_THREADS_THREAD_HPP_FILE

/* public header */

#include <cassert>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Logger/Logger.hpp"
#include "Commons/Exception.hpp"

namespace Commons
{
namespace Threads
{

/** \brief thread wrapper.
 *
 *  wrapper atomatically catches and logs all exceptions. uppon destruction
 *  thread is automatically interrupted and joined.
 */
class Thread: public Base::Threads::ThreadJoiner
{
public:
  /** \brief create and run thread.
   *  \param t callable function object to be used as thread's body.
   */
  template<typename TCallable>
  explicit Thread(const TCallable &t):
    Base::Threads::ThreadJoiner( Operation<TCallable>(t) )
  {
  }

private:
  /** \brief helper object, catching all exceptions.
   */
  template<typename T>
  struct Operation
  {
    /** \brief create object for catching all exceptions.
     *  \param t callable function object to be used as thread's body.
     */
    explicit Operation(const T &t):
      log_("commons.threads.thread"),
      t_(t)
    {
      LOGMSG_DEBUG(log_, "thread started");
    }
    /** \brief catching exceptions from user's thread function.
     */
    void operator()(void)
    {
      LOGMSG_DEBUG(log_, "starting user's thread function");
      // we cannot allow exception to pass on - this is undefined behaviour
      try
      {
        t_();   // run user's code
        LOGMSG_DEBUG(log_, "user's thread function finished without error");
      }
      catch(const boost::thread_interrupted &/*ex*/)
      {
        LOGMSG_WARN(log_, "unhandled thread interruption caught: - aborting...");
      }
      catch(const Commons::Exception &ex)
      {
        LOGMSG_FATAL_S(log_)<<"unexpected Commons::Exception caught: "<<ex.what()<<" - aborting...";
      }
      catch(const std::exception &ex)
      {
        LOGMSG_FATAL_S(log_)<<"unexpected std::exception caught: "<<ex.what()<<" - aborting...";
      }
      catch(...)
      {
        LOGMSG_FATAL(log_, "unexpected, unknown exception caught - aborting...");
        assert(!"unexpected, unknown exception caught");
      }
      LOGMSG_DEBUG(log_, "exiting from user's thread function");
    }

  private:
    Logger::Node log_;
    T            t_;
  }; // struct Operation
}; // class Thread

} // namespace Threads
} // namespace Commons

#endif
