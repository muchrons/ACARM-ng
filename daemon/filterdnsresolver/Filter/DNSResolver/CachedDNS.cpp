/*
 * CachedDNS.cpp
 *
 */
#include "Filter/DNSResolver/CachedDNS.hpp"
#include <deque>
#include <cassert>

using namespace std;


namespace Filter
{
namespace DNSResolver
{

CachedDNS::CachedDNS(unsigned int timeout):
  timeout_(timeout)
{
}

const CachedEntry::Name &CachedDNS::operator[](const IP &ip)
{
  Cache::iterator it=cache_.find(ip);
  if( it==cache_.end() )
  {
    // if entry does not exist yet, add it
    const CachedEntry ce(ip, timeout_);     // translate DNS
    it=cache_.insert( Cache::value_type(ip, ce) ).first;
  }

  assert( it!=cache_.end() );
  return it->second.name_;
}

void CachedDNS::prune(void)
{
  typedef deque<IP> OutdatedList;
  OutdatedList outdated;

  // gather keys of outdated entries
  {
    const time_t now=time(NULL);
    for(Cache::const_iterator it=cache_.begin(); it!=cache_.end(); ++it)
      if(it->second.time_>now)
        outdated.push_back(it->first);
  }

  // remove outdated entries
  for(OutdatedList::const_iterator it=outdated.begin(); it!=outdated.end(); ++it)
    cache_.erase(*it);
}

} // namespace DNSResolver
} // namespace Filter
