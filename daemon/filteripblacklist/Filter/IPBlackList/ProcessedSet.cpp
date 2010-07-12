/*
 * ProcessedSet.cpp
 *
 */
#include "Filter/IPBlackList/ProcessedSet.hpp"

namespace Filter
{
namespace IPBlackList
{

bool ProcessedSet::isProcessed(Persistency::GraphNodePtrNN leaf) const
{
  for(PtrSet::const_iterator it=set_.begin(); it!=set_.end(); ++it)
    if( it->lock().get()==leaf.get() )  // object already processed?
      return true;
  // ok - object not found
  return false;
}

void ProcessedSet::markAsProcessed(Persistency::GraphNodePtrNN leaf)
{
  set_.add(leaf);
}

void ProcessedSet::prune(void)
{
  set_.prune();
}

} // namespace IPBlackList
} // namespace Filter
