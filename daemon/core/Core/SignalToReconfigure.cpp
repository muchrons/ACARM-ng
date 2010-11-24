/*
 * SignalToReconfigure.cpp
 *
 */
#include <stdlib.h>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Logger/Config.hpp"
#include "Core/SignalToReconfigure.hpp"

using namespace Logger;

namespace Core
{

extern "C"
{
static void handle(int signum)
{
  try
  {
    const Logger::Node log("core.signaltoreconfigure.handle");
    try
    {
      LOGMSG_INFO_S(log) << "received signal " << signum;
      LOGMSG_WARN(log, "signal received - calling Logger::Config::reinit()");
      // reconfigure appenders
      Logger::Config::reinitAppenders();
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

SignalToReconfigure::SignalToReconfigure(int signum):
  SignalRegistrator(signum, handle),
  signum_(signum),
  log_("core.signaltoreconfigure")
{
  LOGMSG_DEBUG_S(log_)<<"registered handle for signal "<<signum_;
}

SignalToReconfigure::~SignalToReconfigure(void)
{
  LOGMSG_DEBUG_S(log_)<<"unregistered handle for signal "<<signum_;
}

} // namespace Core
