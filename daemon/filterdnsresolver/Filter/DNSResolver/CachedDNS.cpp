/*
 * CachedDNS.cpp
 *
 */
#include "Filter/DNSResolver/CachedDNS.hpp"
#include <deque>
#include <cassert>
#include "Logger/Logger.hpp"

using namespace std;


namespace Filter
{
namespace DNSResolver
{

CachedDNS::CachedDNS(unsigned int timeout):
  log_("filter.dnsresolver.cacheddns"),
  timeout_(timeout)
{
  LOGMSG_INFO(log_, "creating cache");
}

CachedDNS::Entry CachedDNS::operator[](const IP &ip)
{
  Cache::iterator it=cache_.find(ip);
  if( it==cache_.end() )
  {
    LOGMSG_DEBUG_S(log_)<<"adding new mapping for '"
                        <<ip<<"' for "<<timeout_<<"[s]";
    // if entry does not exist yet, add it
    const CachedEntry ce(ip, timeout_);     // translate DNS
    it=cache_.insert( Cache::value_type(ip, ce) ).first; //TODO, dlaczego tu jest .first, chyba nie kumam tej linijki
    LOGMSG_DEBUG_S(log_)<<it->first<<" maps to '"<<it->second.name_.get()<<"'";
  }

  assert( it!=cache_.end() );
  return Entry( it->second.hasName(), it->second.name_ );
}

void CachedDNS::prune(void)
{
  LOGMSG_DEBUG(log_, "prunning cache");
  typedef deque<IP> OutdatedList;
  OutdatedList outdated;

  // gather keys of outdated entries
  {
    const time_t now=time(NULL);
    for(Cache::const_iterator it=cache_.begin(); it!=cache_.end(); ++it)
      if(it->second.time_<now)
        outdated.push_back(it->first);
  }
//TODO: Why there's an extra collection of elements due to deletion instead of deleting them on the spot?
  LOGMSG_DEBUG_S(log_)<<outdated.size()<<" entries to be removed";
  // remove outdated entries
  for(OutdatedList::const_iterator it=outdated.begin(); it!=outdated.end(); ++it)
    cache_.erase(*it);
}

} // namespace DNSResolver
} // namespace Filter
