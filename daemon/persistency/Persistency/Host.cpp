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

const ReferenceURL *Host::getReferenceURL(void) const
{
  return url_.get();
}

const Host::ReportedServices &Host::getReportedServices(void) const
{
  return services_;
}

const Host::ReportedProcesses &Host::getReportedProcesses(void) const
{
  return processes_;
}

Host::Host(const IPv4              &ip,
           const Netmask_v4        *mask,
           const OperatingSystem    os,
           ReferenceURLPtr          url,
           const ReportedServices  &services,
           const ReportedProcesses &processes):
  ip_(ip),
  mask_( (mask!=NULL)?( new Netmask(*mask) ):NULL ),
  os_(os),
  url_(url),
  services_(services),
  processes_(processes)
{
}

Host::Host(const IPv6              &ip,
           const Netmask_v6        *mask,
           const OperatingSystem    os,
           ReferenceURLPtr          url,
           const ReportedServices  &services,
           const ReportedProcesses &processes):
  ip_(ip),
  mask_( (mask!=NULL)?( new Netmask(*mask) ):NULL ),
  os_(os),
  url_(url),
  services_(services),
  processes_(processes)
{
}

} // namespace Persistency
