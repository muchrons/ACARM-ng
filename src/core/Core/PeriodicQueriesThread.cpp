/*
 * CleanupThread.cpp
 *
 */
#include <cassert>

#include "System/Timer.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Logger/Logger.hpp"
#include "Core/PeriodicQueriesThread.hpp"
#include "Commons/setThreadName.hpp"

namespace Core
{

namespace
{

/** \brief thread-implementation for periodic queries mechanism.
 */
struct PeriodicQueries
{
  /** \brief create object.
   *  \param pt class to run.
   */
  explicit PeriodicQueries(PersistencyPeriodicQueries &pt):
    log_("core.periodicqueries"),
    pt_(&pt)
  {
  }

  /** \brief thread implementation.
   */
  void operator()(void)
  {
    LOGMSG_DEBUG(log_, "thread is up");

    try
    {
      //main loop to check if we have to perform some action
      while(true)
      {
        issue();
        boost::this_thread::sleep( boost::posix_time::minutes(10) ); // sleep 10 minutes
      } // while(true)
    }
    catch(const boost::thread_interrupted &)
    {
      // interrupting on user's request
      LOGMSG_INFO(log_, "interrupting on user's request");
    }

    LOGMSG_DEBUG(log_, "exiting thread noramlly");
  }

private:
  void issue(void)
  {
    try
    {
      assert(pt_!=NULL);
      pt_->issue();
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception cought: '"<<ex.what()<<"' - ignoring it...";
    }
  }

  Logger::Node        log_;
  PersistencyPeriodicQueries *pt_;
}; // struct PeriodicalCleanup
} // unnamed namespace


PeriodicQueriesThread::PeriodicQueriesThread(void):
  log_("core.periodicqueriesthread"),
  th_( PeriodicQueries(pt_) )
{
  LOGMSG_INFO(log_, "periodic queries thread has been started");
  Commons::setThreadName("periodic_queries_thread");
}

PeriodicQueriesThread::~PeriodicQueriesThread(void)
{
  LOGMSG_INFO(log_, "stopping periodic queries thread");
}

} // namespace Core
