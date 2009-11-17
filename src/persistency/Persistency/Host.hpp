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

class Host
{
public:
  typedef asio::ip::address             IP;
  typedef asio::ip::address_v4          IPv4;
  typedef asio::ip::address_v6          IPv6;
  typedef IP                            Netmask;
  typedef IPv4                          Netmask_v4;
  typedef IPv6                          Netmask_v6;
  typedef detail::LimitedNULLString<32> OperatingSystem;
  typedef detail::LimitedNULLString<64> Name;

  virtual ~Host(void);

  const IP &getIP(void) const;

  const Netmask *getNetmask(void) const;

  const OperatingSystem &getOperatingSystem(void) const;

  virtual Name getName(void) const = 0;

  virtual void setName(const Name &name) = 0;

protected:
  Host(const IPv4            &ip,
       const Netmask_v4      *mask,
       const OperatingSystem  os);
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
