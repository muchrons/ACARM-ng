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

void TimeoutedSet::add(const HashSharedPtr &key)
{
  // prevent storing element with the same names
  if(isTimeouted(key))
    return;
  timeouted_.push_back( key );
}

void TimeoutedSet::markRemoved(BackendFacade &bf, const Persistency::IO::DynamicConfig::Owner &owner)
{
  // TODO: notice that only elements that are NOT is processedset at the moment should be removed here.
  //       this is required since Entry() elements withe a given name can repeat.
  Persistency::IO::DynamicConfigAutoPtr dc = bf.createDynamicConfig(owner);
  for(Timeouted::iterator it = timeouted_.begin(); it != timeouted_.end(); ++it)
  {
    // single exception for one entry will not block removing others from DC.
    try
    {
      dc->remove( it->get()->getHash().get() );
    }
    catch(const Persistency::IO::Exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception caught: '"<<ex.what()<<"' - ignoring";
    }
    catch(const std::exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"exception caught: '"<<ex.what()<<"' - ignoring";
    }
  }
  timeouted_.clear();
}

bool TimeoutedSet::isTimeouted(const HashSharedPtr &key) const
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
