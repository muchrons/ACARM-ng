/*
 * IDMap.cpp
 *
 */
#include "Base/Threads/Lock.hpp"
#include "Input/IDMap.hpp"

namespace Input
{

IDMap::IDMap(const Persistency::Analyzer::ID  nextFreeID,
             PersistencyProxy                &pp):
  nextFreeID_(nextFreeID),
  pp_(pp)
{
}

Persistency::Analyzer::ID IDMap::get(const std::string &inputID)
{
  Base::Threads::Lock lock(mutex_);
  // has already this entry?
  {
    MapType::const_iterator it=map_.find(inputID);
    if( it!=map_.end() )
      return it->second;
  }
  // new entry has to be added then!
  const Persistency::Analyzer::ID id=nextFreeID_++; // switch to next id stright away
                                                    // to ensure fast recovery from
                                                    // persistency inconsistent errors.
  pp_.saveMapping(inputID, id);     // save mapping to persistent storage
  map_[inputID]=id;                 // add run-time mapping
  return id;                        // return newly added ID
}

} // namespace Input
