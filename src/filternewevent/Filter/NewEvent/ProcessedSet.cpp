/*
 * ProcessedSet.cpp
 *
 */
#include "Filter/NewEvent/ProcessedSet.hpp"

namespace Filter
{
namespace NewEvent
{

bool ProcessedSet::update(const Hash &hash, const unsigned int timeout)
{
  // TODO: use const_iterator
  for(Set::iterator it=set_.begin(); it!=set_.end(); ++it)
  {
    // TODO: copy is not needed here; use const-ref instead
    const Hash::HashData hashData = (*it).get()->getHash();
    // TODO: 'Hash' has '==' defined as well
    if(hashData==hash.getHash())
    {
      markAsProcessed(*it, timeout);
      return true;
    }
  }
  // object not found
  return false;
}

void ProcessedSet::markAsProcessed(const EntrySharedPtr &entryPtr, const unsigned int timeout)
{
  set_.update(entryPtr, timeout);
}

void ProcessedSet::prune(void)
{
  set_.prune();
}

} // namespace Filter
} // namespace NewEvent
