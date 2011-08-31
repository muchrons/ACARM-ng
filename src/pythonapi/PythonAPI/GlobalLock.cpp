/*
 * GlobalLock.cpp
 *
 */
#include "System/Threads/SafeInitLocking.hpp"
#include "Base/Threads/RecursiveMutex.hpp"
#include "Logger/Logger.hpp"
#include "PythonAPI/GlobalLock.hpp"

namespace PythonAPI
{

namespace
{
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_safeInitMutex); // initialized at compile time

// global python mutex
Base::Threads::RecursiveMutex &getGlobalMutex(void)
{
  System::Threads::SafeInitLock lock(g_safeInitMutex);  // race condition prevention
  static Base::Threads::RecursiveMutex mutex;           // init uppon first usage
  return mutex;                                         // return initialized mutex
} // getGlobalMutex()
} // unnamed namespace


GlobalLock::GlobalLock(void):
  log_("pythonapi.globallock"),
  lock_( getGlobalMutex() )
{
  LOGMSG_DEBUG(log_, "PythonAPI's global mutex is LOCKED now");
}

GlobalLock::~GlobalLock(void)
{
  LOGMSG_DEBUG(log_, "unlocking PythonAPI's global mutex...");
}

} // namespace PythonAPI
