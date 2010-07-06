/*
 * IDMap.cpp
 *
 */
#include "Base/Threads/Lock.hpp"
#include "Input/detail/IDMap.hpp"

namespace Input
{
namespace detail
{

IDMap::IDMap(const Persistency::Analyzer::ID nextFreeID):
  nextFreeID_(nextFreeID)
{
}

Persistency::Analyzer::ID IDMap::get(IDPersistencyProxyCommon   &ppc,
                                     IDPersistencyProxySpecific &pps,
                                     const std::string          &inputID)
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
  ppc.saveNextFreeID(nextFreeID_);  // save new value of next free ID
  pps.saveMapping(inputID, id);     // save mapping to persistent storage
  map_[inputID]=id;                 // add run-time mapping
  return id;                        // return newly added ID
}

} // namespace detail
} // namespace Input
