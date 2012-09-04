/*
 * CachedDNS.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_CACHEDDNS_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_CACHEDDNS_HPP_FILE

#include <map>
#include <boost/noncopyable.hpp>
#include <ctime>

#include "Logger/Node.hpp"
#include "Filter/DNSResolver/CachedEntry.hpp"


namespace Filter
{
namespace DNSResolver
{

/** \brief DNS cache facility.
 */
class CachedDNS: private boost::noncopyable
{
public:
  /** \brief repreesntation of IP. */
  typedef Persistency::Host::IP              IP;
  /** \brief DNS name and information if given IP has reverse DNS. */
  typedef std::pair<bool, CachedEntry::Name> Entry;

  /** \brief create cache with a given timeout.
   *  \param timeout time for each cached entry to exist in cache.
   */
  explicit CachedDNS(unsigned int timeout);
  /** \brief get reverse-dns for given IP.
   *  \param ip ip to query for.
   *  \return pair with bool value telling if IP has reverse DNS and
   *          name, if present.
   *
   *  this call is not const, since it caches obtained results. if given
   *  ip appears more often in a short time window (defined by timeout value)
   *  it is read from cache, instead of being re-queried. this make reverse-dns
   *  mapping much faster.
   */
  Entry operator[](const IP &ip);

private:
  typedef std::map<IP, CachedEntry> Cache;

  /** \brief periodically call prune
   */
  void pruneCache(void);

  /** \brief remove outdated entries from cache.
   *  \note this is a separete call for performance reasons.
   */
  void prune(void);

  time_t             nextPrune_;
  const Logger::Node log_;
  unsigned int       timeout_;
  Cache              cache_;
}; // class CachedDNS

} // namespace DNSResolver
} // namespace Filter

#endif
