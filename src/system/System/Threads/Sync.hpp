/*
 * Sync.hpp
 *
 */
#ifndef INCLUDE_SYSTEM_THREADS_SYNC_HPP_FILE
#define INCLUDE_SYSTEM_THREADS_SYNC_HPP_FILE

/* public header */

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

#include "System/SharedPtrNotNULL.hpp"

namespace System
{
namespace Threads
{

/** \brief class for synchronization between slave and master threads.
 *
 *  master runs start() and waits for result with waitForDone().
 *  slave waits for request on waitForStart() and signals end of work with done().
 */
class Sync
{
public:
  /** \brief initialize data.
   */
  Sync(void);

  /** \brief send slave start signal.
   */
  void start(void);
  /** \brief holds slave until master sends start.
   */
  void waitForStart(void);

  /** \brief mark as done to master.
   */
  void done(void);
  /** \brief holds master until slave finishes task.
   */
  void waitForDone(void);

private:
  boost::mutex              mutex_;
  bool                      startFlag_;
  boost::condition_variable start_;
  bool                      doneFlag_;
  boost::condition_variable done_;
}; // class Sync


/** \brief pointer to synchronization object. */
typedef System::SharedPtrNotNULL<Sync> SyncPtrNN;

} // namespac Threads
} // namespac System

#endif
