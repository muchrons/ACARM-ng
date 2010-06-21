/*
 * IgnoreSignal.cpp
 *
 */
#include <stdlib.h>
#include <cassert>

#include "Logger/Logger.hpp"
#include "Core/IgnoreSignal.hpp"

using namespace Logger;

namespace Core
{

extern "C"
{
static void handle(int signum)
{
  // ignore this signal
  try
  {
    Node log("core.ignoresignal.handle");
    LOGMSG_INFO_S(log) << "signal " << signum << " received - ignoring...";
  }
  catch(...)
  {
    // nothing can be done here - simply ignore this eception...
  }
} // handle()
} // extern "C"

IgnoreSignal::IgnoreSignal(int signum):
  SignalRegistrator(signum, handle),
  signum_(signum),
  log_("core.ignoresignal")
{
  LOGMSG_DEBUG_S(log_) << "signal " << signum_ << " - ignored from now on";
}

IgnoreSignal::~IgnoreSignal(void)
{
  LOGMSG_DEBUG_S(log_) << "signal " << signum_ << " - original handler restored";
}

} // namespace Core
