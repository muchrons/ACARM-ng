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
  signals_(&threads_)
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
