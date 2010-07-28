/*
 * CleanupThread.cpp
 *
 */
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
    bool quit=cleanup();    // run initial cleanup
    while(!quit)
    {
      // TODO: make this sleep time compile-time configuration
      boost::this_thread::sleep( boost::posix_time::seconds(24*3600) ); // sleep one day
      quit=cleanup();                                                   // run cleanup
    } // while(!quit)
    LOGMSG_DEBUG(log_, "thread is exiting normally");
  }

private:
  bool cleanup(void)
  {
    try
    {
      assert(pc_!=NULL);
      pc_->cleanup();
    }
    catch(const boost::thread_interrupted &)
    {
      // interrupting on user's request
      LOGMSG_INFO(log_, "interrupting on user's request");
      return true;
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception cought: '"<<ex.what()<<"' - ignoring it...";
    }
    // continue work
    return false;
  }

  Logger::Node        log_;
  PersistencyCleanup *pc_;
}; // struct PeriodicalCleanup
} // unnamed namespace


CleanupThread::CleanupThread(void):
  log_("core.cleanupthread"),
  th_( PeriodicalCleanup(pc_) )
{
  LOGMSG_INFO(log_, "cleanup thread has been started");
}

CleanupThread::~CleanupThread(void)
{
  LOGMSG_INFO(log_, "stopping cleanup thread");
}

} // namespace Core
