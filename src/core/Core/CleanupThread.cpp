/*
 * CleanupThread.cpp
 *
 */
#include <cassert>

#include "System/Timer.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Logger/Logger.hpp"
#include "Core/CleanupThread.hpp"
#include "Commons/setThreadName.hpp"

namespace Core
{

namespace
{

/** \brief thread-implementation for periodic cleanup mechanism.
 */
struct PeriodicalCleanup
{
  /** \brief create object.
   *  \param pc cleanup to be used.
   */
  explicit PeriodicalCleanup(PersistencyCleanup &pc):
    log_("core.periodicalcleanup"),
    pc_(&pc)
  {
  }

  /** \brief thread implementation.
   */
  void operator()(void)
  {
    LOGMSG_DEBUG(log_, "thread is up");

    try
    {
      cleanup();            // run initial cleanup

      while(true)
      {
        const size_t hours=ConfigIO::Singleton::get()->generalConfig().getCleanupInterval();
        assert(hours>0);    // prevent busy-loops (this condition should be already enforced by the config)
        LOGMSG_INFO_S(log_)<<"next cleanup in "<<hours<<" hours";
        const unsigned int oneHour=60*60;                                       // length of one our in seconds
        boost::this_thread::sleep( boost::posix_time::seconds(hours*oneHour) ); // sleep until next cleanup
        LOGMSG_INFO(log_, "cleanup time has come");
        System::Timer t;
        cleanup();
        // add log if cleanup took over 25% of time between the calls
        const double elapsed=t.elapsed();
        if( elapsed>(0.25*hours*oneHour) )
          LOGMSG_WARN(log_, "cleanup took over 25% of time between next calls - consider calling it more often");
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
  void cleanup(void)
  {
    try
    {
      assert(pc_!=NULL);
      pc_->cleanup();
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception cought: '"<<ex.what()<<"' - ignoring it...";
    }
  }

  Logger::Node        log_;
  PersistencyCleanup *pc_;
}; // struct PeriodicalCleanup
} // unnamed namespace


CleanupThread::CleanupThread(void):
  log_("core.cleanupthread"),
  pc_( ConfigIO::Singleton::get()->generalConfig().getCleanupOlder() ),
  th_( PeriodicalCleanup(pc_) )
{
  LOGMSG_INFO(log_, "cleanup thread has been started");
  Commons::setThreadName("cleanup_thread");
}

CleanupThread::~CleanupThread(void)
{
  LOGMSG_INFO(log_, "stopping cleanup thread");
}

} // namespace Core
