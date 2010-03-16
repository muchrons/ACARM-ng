/*
 * IPTypes.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IPTYPES_HPP_FILE
#define INCLUDE_PERSISTENCY_IPTYPES_HPP_FILE

/* public header */

#include <boost/asio/ip/address.hpp>

namespace Persistency
{

/** \brief common IP typedefs.
 */
template<typename CRTP>
struct IPTypes
{
  /** \brief any IP address type.
   */
  typedef boost::asio::ip::address      IP;
  /** \brief IPv4 address.
   */
  typedef boost::asio::ip::address_v4   IPv4;
  /** \brief IPv6 address.
   */
  typedef boost::asio::ip::address_v6   IPv6;
  /** \brief any network mask.
   */
  typedef IP                            Netmask;
  /** \brief network mask for IPv4.
   */
  typedef IPv4                          Netmask_v4;
  /** \brief network mask for IPv6.
   */
  typedef IPv6                          Netmask_v6;
}; // struct IPTypes

} // namespace Persistency

#endif
