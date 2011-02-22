/*
 * TimeoutedSet.cpp
 *
 */

#include "Filter/NewEvent/TimeoutedSet.hpp"

namespace Filter
{
namespace NewEvent
{

void TimeoutedSet::add(std::string &key)
{
  // TODO
  timeouted_.push_back(key);
}
void TimeoutedSet::prune(BackendFacade *bf, Persistency::IO::DynamicConfig::Owner &owner)
{
  // TODO: iterate collection and call removeFromDynamicConfig() function for each element
  //       then delete element from collection
  Persistency::IO::DynamicConfigAutoPtr dc = bf->createDynamicConfig(owner);
  for(Timeouted::iterator it = timeouted_.begin(); it != timeouted_.end(); ++it)
  {
    // TODO: throw exception when there is no entry saved in dynamic config
    dc->remove(*it);
  }
  timeouted_.clear();
}

} // namespace Filter
} // namespace NewEvent
