/*
 * CachedEntry.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_CACHEDENTRY_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_CACHEDENTRY_HPP_FILE

#include <ctime>

#include "Persistency/Host.hpp"


namespace Filter
{
namespace DNSResolver
{

struct CachedEntry
{
  typedef Persistency::Host::IP   IP;
  typedef Persistency::Host::Name Name;
  typedef time_t                  Time;

  CachedEntry(const IP &ip, const Name &name, unsigned int timeout):
    ip_(ip),
    name_(name),
    time_( time(NULL)+timeout )
  {
  }

  IP   ip_;
  Name name_;
  Time time_;
}; // struct CachedEntry


struct CachedEntrySWO
{
  bool operator()(const CachedEntry &left, const CachedEntry &right) const
  {
    return left.ip_<right.ip_;
  }
}; // struct CachedEntrySWO

} // namespace DNSResolver
} // namespace Filter

#endif
