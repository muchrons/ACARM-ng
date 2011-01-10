/*
 * CachedEntry.hpp
 *
 */
#ifndef INCLUDE_FILTER_DNSRESOLVER_CACHEDENTRY_HPP_FILE
#define INCLUDE_FILTER_DNSRESOLVER_CACHEDENTRY_HPP_FILE

#include <ctime>

#include "Persistency/Host.hpp"
#include "Filter/DNSResolver/ExceptionUnableToResolve.hpp"

namespace Filter
{
namespace DNSResolver
{

/** \brief helper object, holding cached entry and quering for IPs DNS name.
 */
struct CachedEntry
{
  /** \brief type repreesnting IP. */
  typedef Persistency::Host::IP   IP;
  /** \brief DNS name for a given IP. */
  typedef Persistency::Host::Name Name;
  /** \brief time representation for timeouts. */
  typedef time_t                  Time;

  /** \brief revers-DNS IP and save result.
   *  \param ip      IP to reverse-DNS.
   *  \param timeout maximum amount of time for this entry to be valid.
   *
   *  this call automatically performs reverse DNS for given IP.
   */
  CachedEntry(const IP &ip, unsigned int timeout):
    time_( time(NULL)+timeout )
  {
    resolve(ip);
  }
  /** \brief checks if IP has reverse DNS.
   *  \return true if entry repreesnts valid, reverse DNS mapping, false
   *          otherwise.
   */
  bool hasName(void) const
  {
    return hasName_;
  }

  Name name_;   ///< reversed DNS name
  Time time_;   ///< deadline for given entry's validity.

private:
  void resolve(const IP &ip);

  bool hasName_;
}; // struct CachedEntry

} // namespace DNSResolver
} // namespace Filter

#endif
