/*
 * TimeoutedSet.cpp
 *
 */

#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

void TimeoutedSet::add(const Entry::Hash &key)
{
  // TODO
  timeouted_.push_back(key);
}

void TimeoutedSet::prune(BackendFacade *bf, Persistency::IO::DynamicConfig::Owner &owner)
{
  // TODO: notice that only elements that are NOT is processedset at the moment should be removed here.
  //       this is required since Entry() elements withe a given name can repeat.
  // TODO: iterate collection and call removeFromDynamicConfig() function for each element
  //       then delete element from collection
  Persistency::IO::DynamicConfigAutoPtr dc = bf->createDynamicConfig(owner);
  for(Timeouted::iterator it = timeouted_.begin(); it != timeouted_.end(); ++it)
  {
    // TODO: add try{}catch(...) around loop's body to ensure single exception for one entry
    //       will not block removing others from DC.
    // TODO: throw exception when there is no entry saved in dynamic config
    dc->remove(*it);
  }
  timeouted_.clear();
}

bool TimeoutedSet::isTimeouted(const Entry::Hash &key) const
{
  for(Timeouted::const_iterator it = timeouted_.begin(); it != timeouted_.end(); ++it)
  {
    if(*it==key)
      return true;
  }
  // object not found
  return false;
}

} // namespace Filter
} // namespace NewEvent
