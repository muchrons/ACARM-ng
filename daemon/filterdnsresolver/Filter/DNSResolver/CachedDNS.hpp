/*
 * CachedDNS.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_CACHEDDNS_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_CACHEDDNS_HPP_FILE

#include <map>
#include <boost/noncopyable.hpp>

#include "Filter/DNSResolver/CachedEntry.hpp"


namespace Filter
{
namespace DNSResolver
{

class CachedDNS: private boost::noncopyable
{
public:
  typedef Persistency::Host::IP IP;

  explicit CachedDNS(unsigned int timeout);

  const CachedEntry::Name &operator[](const IP &ip);

  void prune(void);

private:
  typedef std::map<IP, CachedEntry> Cache;

  unsigned int timeout_;
  Cache        cache_;
}; // class CachedDNS

} // namespace DNSResolver
} // namespace Filter

#endif
