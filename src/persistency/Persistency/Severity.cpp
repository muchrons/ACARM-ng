/*
 * Severity.cpp
 *
 */
#include <cassert>

#include "Persistency/Severity.hpp"

namespace Persistency
{

Severity::~Severity(void)
{
}

SeverityLevel Severity::getLevel(void) const
{
  return sl_;
}

const char *Severity::getName(void) const
{
  switch( sl_.toInt() )
  {
    case SeverityLevel::DEBUG:    return "debug";
    case SeverityLevel::INFO:     return "info";
    case SeverityLevel::NOTICE:   return "notice";
    case SeverityLevel::WARNING:  return "warning";
    case SeverityLevel::PROBLEM:  return "problem";
    case SeverityLevel::ERROR:    return "error";
    case SeverityLevel::CRITICAL: return "critical";
  } // switch(SeverityLevel)

  // we should never reach here
  assert(!"name for invalid severity requested");
  return "<" "????" ">";
}

Severity::Severity(const SeverityLevel sl):
  sl_(sl)
{
}

} // namespace Persistency
