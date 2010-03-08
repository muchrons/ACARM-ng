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

  CachedEntry(const IP &ip, unsigned int timeout):
    time_( time(NULL)+timeout )
  {
    resolve(ip);
  }

  bool hasName(void) const
  {
    return hasName_;
  }

  Name name_;
  Time time_;

private:
  void resolve(const IP &ip);

  bool hasName_;
}; // struct CachedEntry

} // namespace DNSResolver
} // namespace Filter

#endif
