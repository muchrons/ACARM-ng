/*
 * TimeoutedSet.cpp
 *
 */
#include "Filter/NewEvent/TimeoutedSet.hpp"
#include "Persistency/IO/Exception.hpp"

namespace Filter
{
namespace NewEvent
{

TimeoutedSet::TimeoutedSet():
  log_("filter.newevent")
{
}

void TimeoutedSet::add(const Hash &key)
{
  // prevent storing elements with the same names
  if( !isTimeouted(key) )
  timeouted_.push_back(key);
}

void TimeoutedSet::markRemoved(BackendFacade &bf, const Persistency::IO::DynamicConfig::Owner &owner)
{
  // NOTE: only elements that are NOT in processed set at the moment should be removed here.
  //       Entry() elements with a given name can't repeat, EntryProcessor implementation
  //       prevents adding Entry elements with the same names to the ProcessedSet collection.
  Persistency::IO::DynamicConfigAutoPtr dc=bf.createDynamicConfig(owner);
  for(Timeouted::iterator it=timeouted_.begin(); it!=timeouted_.end(); ++it)
  {
    // single exception for one entry will not block removing others from DC.
    try
    {
      dc->remove( it->getHash().get() );
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception caught: '"<<ex.what()<<"' - ignoring";
    }
  }
  timeouted_.clear();
}

bool TimeoutedSet::isTimeouted(const Hash &key) const
{
  return std::find(timeouted_.begin(), timeouted_.end(), key) != timeouted_.end();
}

} // namespace Filter
} // namespace NewEvent
