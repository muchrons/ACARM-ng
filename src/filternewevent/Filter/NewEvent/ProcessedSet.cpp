/*
 * ProcessedSet.cpp
 *
 */
#include "Filter/NewEvent/ProcessedSet.hpp"

namespace Filter
{
namespace NewEvent
{

bool ProcessedSet::update(const Hash &hash, unsigned int timeout)
{
  for(Set::iterator it = set_.begin(); it != set_.end(); ++it)
  {
    const Hash::HashData hashData = (*it).get()->getHash();
    if(hashData==hash.getHash())
    {
      markAsProcessed(*it, timeout);
      return true;
    }
  }
  // object not found
  return false;
}

void ProcessedSet::markAsProcessed(const EntrySharedPtr &entryPtr, unsigned int timeout)
{
  set_.update(entryPtr, timeout);
}

void ProcessedSet::prune(void)
{
  set_.prune();
}

} // namespace Filter
} // namespace NewEvent
