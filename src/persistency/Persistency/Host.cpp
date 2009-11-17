/*
 * Host.cpp
 *
 */

#include "Persistency/Host.hpp"

namespace Persistency
{

Host::~Host(void)
{
}

const Host::IP &Host::getIP(void) const
{
  return ip_;
}

const Host::Netmask *Host::getNetmask(void) const
{
  return mask_.get();
}

const Host::OperatingSystem &Host::getOperatingSystem(void) const
{
  return os_;
}

Host::Host(const IPv4            &ip,
           const Netmask_v4      *mask,
           const OperatingSystem  os):
  ip_(ip),
  mask_( (mask!=NULL)?( new Netmask(*mask) ):NULL ),
  os_(os)
{
}

Host::Host(const IPv6            &ip,
           const Netmask_v6      *mask,
           const OperatingSystem  os):
  ip_(ip),
  mask_( (mask!=NULL)?( new Netmask(*mask) ):NULL ),
  os_(os)
{
}

} // namespace Persistency
