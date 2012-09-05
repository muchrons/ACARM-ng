/*
 * MemoryUseChecker.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Core/MemoryUseChecker.hpp"

namespace Core
{

MemoryUseChecker::MemoryUseChecker(const Core::Types::SignedNodesFifo & queue):
  log_("core.memoryusechecker"),
  queue_(queue),
  maxAlerts_(ConfigIO::Singleton::get()->generalConfig().getAlertsLimit())
{
}

bool MemoryUseChecker::alertsLimitExceeded() const
{
  if (queue_.size()>maxAlerts_)
    return true;
  return false;
}

} // namespace Core
