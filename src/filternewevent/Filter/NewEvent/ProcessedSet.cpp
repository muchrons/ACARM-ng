/*
 * ProcessedSet.cpp
 *
 */
#include "Filter/NewEvent/ProcessedSet.hpp"

namespace Filter
{
namespace NewEvent
{

bool ProcessedSet::isProcessed(const Entry &entry) const
{
  for(Set::const_iterator it = set_.begin(); it != set_.end(); ++it)
  {
    if(*it->get()==entry)
      return true;
  }
  // object not found
  return false;
}

void ProcessedSet::markAsProcessed(const EntrySharedPtr &entry, unsigned int timeout)
{
  set_.update(entry, timeout);
}

void ProcessedSet::prune(void)
{
  set_.prune();
}

} // namespace Filter
} // namespace NewEvent
