/*
 * CleanupThread.cpp
 *
 */
#include <cassert>

#include "ConfigIO/Singleton.hpp"
#include "Logger/Logger.hpp"
#include "Core/CleanupThread.hpp"

namespace Core
{

namespace
{
struct PeriodicalCleanup
{
  explicit PeriodicalCleanup(PersistencyCleanup &pc):
    log_("core.periodicalcleanup"),
    pc_(&pc)
  {
  }

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
        cleanup();
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
}

CleanupThread::~CleanupThread(void)
{
  LOGMSG_INFO(log_, "stopping cleanup thread");
}

} // namespace Core
