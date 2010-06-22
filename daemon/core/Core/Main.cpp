/*
 * Main.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Core/Processors.hpp"
#include "Core/Sources.hpp"
#include "Core/Main.hpp"

namespace Core
{

Main::Main(void):
  log_("core.main"),
  nullSignals_(NULL),   // ignore all interruptions at the begining
  restorer_(queue_),    // restore data content
  threads_(queue_),     // run processing threads
  // (non-explicit initialization is being done here)
  signals_(&threads_)   // after all initialization is done, register real handlers
{
  LOGMSG_INFO(log_, "core is up and running");
}

Main::~Main(void)
{
  LOGMSG_INFO(log_, "destroying core");
}

void Main::waitUntilDone(void)
{
  threads_.waitUntilDone();
}

void Main::stop(void)
{
  threads_.stop();
}

} // namespace Core
