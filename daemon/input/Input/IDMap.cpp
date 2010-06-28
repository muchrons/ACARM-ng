/*
 * IDMap.cpp
 *
 */
#include "Base/Threads/Lock.hpp"
#include "Input/IDMap.hpp"

namespace Input
{

IDMap::IDMap(void):
  nextFreeID_(0)    // TODO: should be read from data base
{
}

Persistency::Analyzer::ID IDMap::operator[](const std::string &originalID)
{
  Base::Threads::Lock lock(mutex_);
  // has already this entry?
  {
    MapType::const_iterator it=map_.find(originalID);
    if( it!=map_.end() )
      return it->second;
  }
  // new entry has to be added then!
  // TODO: this mapping must be saved persistently!
  map_[originalID]=nextFreeID_;
  return nextFreeID_++;
}

} // namespace Input
