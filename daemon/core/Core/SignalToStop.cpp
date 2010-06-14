/*
 * SignalToStop.cpp
 *
 */
#include <stdlib.h>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Core/SignalToStop.hpp"

using namespace Logger;

namespace Core
{

namespace
{
WorkThreads *g_wt=NULL;
} // unnamed namespace

extern "C"
{
static void handle(int)
{
  try
  {
    const Logger::Node log("core.handle");
    try
    {
      // ignore if nothing is registered
      if(g_wt==NULL)
      {
        LOGMSG_WARN(log, "no handler registered");
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
} // handle()
} // extern "C"

SignalToStop::SignalToStop(int signum, WorkThreads *wt):
  SignalRegistrator(signum, handle)
{
  // NOTE: wt CAN be NULL!
  g_wt=wt;
}

SignalToStop::~SignalToStop(void)
{
  g_wt=NULL;
}

} // namespace Core
