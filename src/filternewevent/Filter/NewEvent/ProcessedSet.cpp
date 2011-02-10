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
