/*
 * GlobalInit.cpp
 *
 */
#include "System/GlobalInit.hpp"

namespace
{
// gloabl mutex, initialized during program's compilation to be used
// for all global init instances.
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_mutex);
} // unnamed namespace

namespace System
{
namespace detail
{

Threads::SafeInitLock::MutexType &getGlabalInitMutex(void)
{
  return g_mutex;
}

} // namespace detail
} // namespace System
