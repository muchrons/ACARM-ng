/*
 * CachedDNS.cpp
 *
 */
#include <cassert>

#include "Logger/Logger.hpp"
#include "Filter/DNSResolver/CachedDNS.hpp"

using namespace std;


namespace Filter
{
namespace DNSResolver
{

CachedDNS::CachedDNS(unsigned int timeout):
  nextPrune_(0),
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
                        <<ip<<"' with timeout "<<timeout_<<"[s]";
    // if entry does not exist yet, add it
    const CachedEntry ce(ip, timeout_);     // translate DNS
    it=cache_.insert( Cache::value_type(ip, ce) ).first;
  }

  pruneCache();

  LOGMSG_DEBUG_S(log_)<<it->first<<" maps to '"<<it->second.name_.get();
  assert( it!=cache_.end() );
  return Entry( it->second.hasName(), it->second.name_ );
}

void CachedDNS::pruneCache(void)
{
  // do this only once per some time
  const time_t now=time(NULL);
  if(nextPrune_>now)
    return;
  prune();
  nextPrune_=now+60;                   // it does not make sense to make it more often than once per 60[s]
}

void CachedDNS::prune(void)
{
  LOGMSG_DEBUG(log_, "prunning cache");
  const time_t now=time(NULL);

  Cache::iterator it=cache_.begin();
  while( it!=cache_.end() )
  {
    Cache::iterator tmp=it++;       // move to next entry but save current one temporary
    if(tmp->second.time_<now)       // check if entry is not outdated
    {
      LOGMSG_DEBUG_S(log_)<<"removing entry for IP "<<tmp->first<<" \""<<tmp->second.name_.get()  <<"\"";
      cache_.erase(tmp);            // if yes, remove it.
    }
  } // while(!end)
}

} // namespace DNSResolver
} // namespace Filter
