/*
 * Host.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Persistency/Host.hpp"
#include "Base/Threads/Lock.hpp"
#include "Base/ViaPointer.hpp"
#include "Commons/ViaCollection.hpp"
#include "Logger/Logger.hpp"

using std::stringstream;


namespace Persistency
{

Host::Host(const IPv4              &ip,
           const Netmask_v4        *mask,
           const OperatingSystem    os,
           ReferenceURLPtr          url,
           const ReportedServices  &services,
           const ReportedProcesses &processes,
           const Name              &name):
  ip_(ip),
  mask_( (mask!=NULL)?( new Netmask(*mask) ):NULL ),
  os_(os),
  name_(name),
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
           const ReportedProcesses &processes,
           const Name              &name):
  ip_(ip),
  mask_( (mask!=NULL)?( new Netmask(*mask) ):NULL ),
  os_(os),
  name_(name),
  url_(url),
  services_(services),
  processes_(processes)
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

const Host::Name &Host::getName(void) const
{
  // although it looks as returing pointer to non-thread safe code, it is fine, since
  // this pointer may be ither NULL (always thread-correct) or exact value, that
  // is set just once. trying to set olready-set value will always throw. so in fact
  // this pointer may transit NULL->0xC0DE only once and will have the same value
  // until this object lives.
  Base::Threads::Lock lock(mutex_);
  return name_;
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

bool Host::operator==(const Host &other) const
{
  if(this==&other)
    return true;

  if( getIP()!=other.getIP() )
    return false;
  if( !Base::ViaPointer::equal( getNetmask(), other.getNetmask() ) )
    return false;
  if( getOperatingSystem()!=other.getOperatingSystem() )
    return false;
  if( getName()!=other.getName() )
    return false;
  if( !Base::ViaPointer::equal( getReferenceURL(), other.getReferenceURL() ) )
    return false;
  if( !Commons::ViaCollection::equal( getReportedServices(),
                                      other.getReportedServices() ) )
    return false;
  if( !Commons::ViaCollection::equal( getReportedProcesses(),
                                      other.getReportedProcesses() ) )
    return false;
  // if everything's the same, return.
  return true;
}

void Host::setName(const Name &name)
{
  // log setting host's name
  {
    Logger::Node log("persistency.host");
    stringstream ss;
    ss<<"host "<<ip_<<" - setting name to '"<<name.get()<<"'";
    LOGMSG_INFO(log, ss.str().c_str() );
  }

  Base::Threads::Lock lock(mutex_);
  // NOTE: checking for not-setting multiple times is crutial here, since
  //       getName() returns pointer to local variable and therefor it must
  //       be asured it never changes.
  if( name_.get()!=NULL )
    throw Exception(SYSTEM_SAVE_LOCATION, "host's name already resolved");

  // add new host entry
  name_=name;
  assert( name_.get()!=NULL );
}

} // namespace Persistency
