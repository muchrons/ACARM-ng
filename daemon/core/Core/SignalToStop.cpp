/*
 * SignalToStop.cpp
 *
 */
#include <stdlib.h>
#include <cassert>

#include "Core/SignalToStop.hpp"

using namespace Logger;

namespace Core
{

namespace
{
Main *g_main=NULL;
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
      if(g_main==NULL)
      {
        LOGMSG_WARN(log, "no handler registered");
        return;
      }
      // send abort
      LOGMSG_WARN(log, "signal received - calling Core::Main::stop()");
      g_main->stop();
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

SignalToStop::SignalToStop(int signum, Main &main):
  SignalRegistrator(signum, handle)
{
  assert(g_main==NULL || g_main==&main);
  g_main=&main;
}

SignalToStop::~SignalToStop(void)
{
  g_main=NULL;
}

} // namespace Core
