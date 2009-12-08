/*
 * Host.cpp
 *
 */
#include <cassert>

#include "Persistency/Host.hpp"
#include "Base/Threads/Lock.hpp"

namespace Persistency
{

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

const Host::Name *Host::getName(void) const
{
  // although it looks as returing pointer to non-thread safe code, it is fine, since
  // this pointer may ge ither NULL (always thread-correct) or exact value, that
  // is set just once. trying to set olready-set value will always throw. so in fact
  // this pointer may transit NULL->0xC0DE only once and will have the same value
  // until this object lives.
  Base::Threads::Lock lock(mutex_);
  return name_.get();
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

void Host::setName(const Name &name)
{
  // TODO: add logging of name resolved event

  Base::Threads::Lock lock(mutex_);
  // NOTE: checking for not-setting multiple times is crutial here, since
  //       getName() returns pointer to local variable and therefor it must
  //       be asured it never changes.
  if( name_.get()!=NULL )
    throw Exception(__FILE__, "host's name already resolved");

  // add new host entry
  name_.reset( new Name(name) );
  assert( name_.get()!=NULL );
}

} // namespace Persistency
