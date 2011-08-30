/*
 * GlobalLock.cpp
 *
 */
#include "Base/Threads/RecursiveMutex.hpp"
#include "Logger/Logger.hpp"
#include "PythonAPI/GlobalLock.hpp"

namespace PythonAPI
{

namespace
{
// global python mutex
Base::Threads::RecursiveMutex g_mutex;
} // unnamed namespace


GlobalLock::GlobalLock(void):
  log_("pythonapi.globallock"),
  lock_(g_mutex)
{
  LOGMSG_DEBUG(log_, "PythonAPI's global mutex is LOCKED now");
}

GlobalLock::~GlobalLock(void)
{
  LOGMSG_DEBUG(log_, "unlocking PythonAPI's global mutex...");
}

} // namespace PythonAPI
