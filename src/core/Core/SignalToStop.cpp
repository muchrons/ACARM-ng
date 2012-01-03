/*
 * SignalToStop.cpp
 *
 */
#include <stdlib.h>
#include <cassert>

#include "System/Threads/SafeInitLocking.hpp"
#include "Logger/Logger.hpp"
#include "Core/SignalToStop.hpp"

using namespace Logger;

namespace Core
{

namespace
{
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_wtMutex);
WorkThreads *g_wt=NULL;
} // unnamed namespace

extern "C"
{
static void handle(int signum)
{
  // NOTE: this lock must be present while whole signal handling, since setting
  //       g_wt to NULL usually means it is destroyed. to use this pointer we need
  //       to have lock on it all of usage time.
  System::Threads::SafeInitLock lock(g_wtMutex);
  try
  {
    const Logger::Node log("core.signaltostop.handle");
    try
    {
      LOGMSG_INFO_S(log) << "received signal " << signum;
      // ignore if nothing is registered
      if(g_wt==NULL)
      {
        LOGMSG_WARN(log, "no WorkThreads pointer registered - ignoring");
        return;
      }
      // send abort
      LOGMSG_WARN(log, "signal received - calling Core::Main::stop()");
      g_wt->stop();
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log)<<"exception caught: "<<ex.what();
    }
  }
  catch(...)
  {
    // logger initialization failed - nothing more we can do...
  }
  assert(g_wt!=NULL && "g_wt changed in run time - something's wrong...");
} // handle()
} // extern "C"

SignalToStop::SignalToStop(int signum, WorkThreads *wt):
  SignalRegistrator(signum, handle),
  signum_(signum),
  log_("core.signaltostop")
{
  System::Threads::SafeInitLock lock(g_wtMutex);
  // NOTE: wt CAN be NULL!
  g_wt=wt;
  LOGMSG_DEBUG_S(log_)<<"registered handle for signal "<<signum_<<" - handle at address "<<wt;
}

SignalToStop::~SignalToStop(void)
{
  System::Threads::SafeInitLock lock(g_wtMutex);
  g_wt=NULL;
  LOGMSG_DEBUG_S(log_)<<"unregistered handle for signal "<<signum_;
}

} // namespace Core
