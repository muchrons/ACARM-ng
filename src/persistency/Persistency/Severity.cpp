/*
 * Severity.cpp
 *
 */
#include <cassert>

#include "Persistency/Severity.hpp"

namespace Persistency
{

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
    case SeverityLevel::LOW:      return "low";
    case SeverityLevel::MEDIUM:   return "medium";
    case SeverityLevel::HIGH:     return "high";
  } // switch(SeverityLevel)

  // we should never reach here
  assert(!"name for invalid severity requested");
  return "<" "????" ">";
}

Severity::Severity(const SeverityLevel sl):
  sl_(sl)
{
}

bool Severity::operator==(const Severity &other) const
{
  return getLevel()==other.getLevel();
}

} // namespace Persistency
