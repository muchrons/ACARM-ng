/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_HOST_HPP_FILE

/* public header */

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>
#include <boost/asio/ip/address.hpp>

#include "Base/NullValue.hpp"
#include "Base/Threads/ReadWriteMutex.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Commons/LimitedNULLString.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/Service.hpp"
#include "Persistency/Process.hpp"
#include "Persistency/IPTypes.hpp"
#include "Persistency/ExceptionHostNameAlreadySet.hpp"

namespace Persistency
{
namespace IO
{
// forward declaration for friend
class Host;
} // namespace IO


/** \brief host information representation.
 */
class Host: private boost::noncopyable,
            public  boost::equality_comparable<Host>,
            public  IPTypes<Host>
{
public:
  /** \brief operation system name type. */
  typedef Commons::LimitedNULLString<32> OperatingSystem;
  /** \brief host name (DNS entry). */
  typedef Commons::LimitedNULLString<64> Name;
  /** \brief services assigned to host. */
  typedef std::vector<ConstServicePtrNN> Services;
  /** \brief processes assigned to host. */
  typedef std::vector<ConstProcessPtrNN> Processes;

  /** \brief create host entry of IPv4 address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - can be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   *  \param name      DNS name of this host (or NULL if not known).
   */
  Host(const IPv4            &ip,
       const Netmask_v4      *mask,
       const OperatingSystem  os,
       ConstReferenceURLPtr   url,
       const Services        &services,
       const Processes       &processes,
       const Name            &name);
  /** \brief create host entry of IPv6 address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - cen be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   *  \param name      DNS name of this host (or NULL if not known).
   */
  Host(const IPv6            &ip,
       const Netmask_v6      *mask,
       const OperatingSystem  os,
       ConstReferenceURLPtr   url,
       const Services        &services,
       const Processes       &processes,
       const Name            &name);
  /** \brief create host entry of IP address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - cen be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   *  \param name      DNS name of this host (or NULL if not known).
   */
  Host(const IP              &ip,
       const Netmask         *mask,
       const OperatingSystem  os,
       ConstReferenceURLPtr   url,
       const Services        &services,
       const Processes       &processes,
       const Name            &name);

  /** \brief gets IP address.
   *  \return IP address of host.
   */
  const IP &getIP(void) const;
  /** \brief gets network mask of a given hosts's address.
   *  \return network maks of a given host or NULL if not known.
   */
  const Netmask *getNetmask(void) const;
  /** \brief operating system name of a given host.
   *  \return name of an operating system.
   */
  const OperatingSystem &getOperatingSystem(void) const;
  /** \brief gets DNS name of a given host.
   *  \return host name.
   *  \note pointer may be NULL, if name has not been set.
   */
  Name getName(void) const;
  /** \brief gets reference url for this host.
   *  \return reference url to get more info.
   */
  ConstReferenceURLPtr getReferenceURL(void) const;
  /** \brief gets reported services list.
   *  \return vector of reported services.
   */
  const Services &getServices(void) const;
  /** \brief gets reported processes list.
   *  \return vector of reported processes.
   */
  const Processes &getProcesses(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Host &other) const;

private:
  friend class ::Persistency::IO::Host;
  void setName(const Name &name);

  IP                                    ip_;
  Base::NullValue<Netmask>              mask_;
  OperatingSystem                       os_;
  Name                                  name_;
  ConstReferenceURLPtr                  url_;
  Services                              services_;
  Processes                             processes_;
  mutable Base::Threads::ReadWriteMutex mutex_;
}; // class Host


/** \brief smart pointer to host name. */
typedef boost::shared_ptr<Host>                                  HostPtr;

/** \brief smart pointer to host name - const version. */
typedef boost::shared_ptr<const HostPtr::element_type>           ConstHostPtr;

/** \brief smart pointer to host name, checked not to be NULL. */
typedef Commons::SharedPtrNotNULL<HostPtr::element_type>         HostPtrNN;

/** \brief smart pointer to host name, checked not to be NULL - const version. */
typedef Commons::SharedPtrNotNULL<const HostPtrNN::element_type> ConstHostPtrNN;

} // namespace Persistency

#endif
