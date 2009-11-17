/*
 * Host.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_HOST_HPP_FILE
#define INCLUDE_PERSISTENCY_HOST_HPP_FILE

/* public header */

#include <boost/scoped_ptr.hpp>
#include <asio/ip/address.hpp>

#include "Persistency/detail/LimitedNULLString.hpp"

namespace Persistency
{
/** \brief host information representation.
 */
class Host
{
public:
  /** \brief any IP address type.
   */
  typedef asio::ip::address             IP;
  /** \brief IPv4 address.
   */
  typedef asio::ip::address_v4          IPv4;
  /** \brief IPv6 address.
   */
  typedef asio::ip::address_v6          IPv6;
  /** \brief any network mask.
   */
  typedef IP                            Netmask;
  /** \brief network mask for IPv4.
   */
  typedef IPv4                          Netmask_v4;
  /** \brief network mask for IPv6.
   */
  typedef IPv6                          Netmask_v6;
  /** \brief operation system name type.
   */
  typedef detail::LimitedNULLString<32> OperatingSystem;
  /** \brief host name (DNS entry).
   */
  typedef detail::LimitedNULLString<64> Name;

  /** \brief ensure proper destruction when inherited.
   */
  virtual ~Host(void);
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
   *  \note value instead of const reference is returned here intentionaly.
   *        value can be set in runtime, therefor it should be always read
   *        from destination storage.
   */
  virtual Name getName(void) const = 0;
  /** \brief sets name of a given host.
   *  \param name DNS name of a host to be set.
   */
  virtual void setName(const Name &name) = 0;

protected:
  /** \brief create host entry of IPv4 address.
   *  \param ip   ip address.
   *  \param mask network maks of a given host.
   *  \param os   operating system name.
   */
  Host(const IPv4            &ip,
       const Netmask_v4      *mask,
       const OperatingSystem  os);
  /** \brief create host entry of IPv6 address.
   *  \param ip   ip address.
   *  \param mask network maks of a given host.
   *  \param os   operating system name.
   */
  Host(const IPv6            &ip,
       const Netmask_v6      *mask,
       const OperatingSystem  os);

private:
  IP                         ip_;
  boost::scoped_ptr<Netmask> mask_;
  OperatingSystem            os_;
}; // class Host

} // namespace Persistency

#endif
