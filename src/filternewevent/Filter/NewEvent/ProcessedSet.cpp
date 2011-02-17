/*
 * ProcessedSet.cpp
 *
 */

#include "Filter/NewEvent/ProcessedSet.hpp"

namespace Filter
{
namespace NewEvent
{

bool ProcessedSet::isProcessed(Entry &entry) const
{
  const Entry::Name name(entry.getName());
  for(Set::const_iterator it = set_.begin(); it != set_.end(); ++it)
  {
    if(*it==name)
      return true;
  }
  // object not found
  return false;
}

void ProcessedSet::markAsProcessed(Entry &entry, unsigned int timeout)
{
  set_.update(entry.getName(), timeout);
}

void ProcessedSet::prune(void)
{
  set_.prune();
}

} // namespace Filter
} // namespace NewEvent
