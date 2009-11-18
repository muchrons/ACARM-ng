/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_HOST_HPP_FILE

/* public header */

#include "Persistency/Host.hpp"

namespace Persistency
{
namespace Postgres
{
/** \brief host information representation.
 */
class Host: public Persistency::Host
{
public:
  /** \brief create host entry of IPv4 address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - can be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   */
  Host(const IPv4              &ip,
       const Netmask_v4        *mask,
       const OperatingSystem    os,
       ReferenceURLPtr          url,
       const ReportedServices  &services,
       const ReportedProcesses &processes);
  /** \brief create host entry of IPv6 address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - cen be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   */
  Host(const IPv6              &ip,
       const Netmask_v6        *mask,
       const OperatingSystem    os,
       ReferenceURLPtr          url,
       const ReportedServices  &services,
       const ReportedProcesses &processes);

  /** \brief gets DNS name of a given host.
   *  \return host name.
   *  \note value instead of const reference is returned here intentionaly.
   *        value can be set in runtime, therefor it should be always read
   *        from destination storage.
   */
  virtual Name getName(void) const;
  /** \brief sets name of a given host.
   *  \param name DNS name of a host to be set.
   */
  virtual void setName(const Name &name);
}; // class Host

} // namespace Postgres
} // namespace Persistency

#endif
