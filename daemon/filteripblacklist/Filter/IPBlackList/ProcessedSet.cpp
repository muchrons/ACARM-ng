/*
 * ProcessedSet.cpp
 *
 */
#include "Filter/IPBlackList/ProcessedSet.hpp"

namespace Filter
{
namespace IPBlackList
{

bool ProcessedSet::isProcessed(Persistency::GraphNodePtrNN leaf, Persistency::HostPtrNN host) const
{
  const PtrSet::Data tmp( leaf.shared_ptr(), host );
  // notice that only linear search can be used here, since weak_ptrs might
  // become NULL instantly (other processors can free them in a mean time).
  for(PtrSet::const_iterator it=set_.begin(); it!=set_.end(); ++it)
    if(*it==tmp)
      return true;
  // ok - object not found
  return false;
}

void ProcessedSet::markAsProcessed(Persistency::GraphNodePtrNN leaf, Persistency::HostPtrNN host)
{
  set_.add( leaf.shared_ptr(), host );
}

void ProcessedSet::prune(void)
{
  set_.prune();
}

} // namespace IPBlackList
} // namespace Filter
