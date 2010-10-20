/*
 * Host.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Persistency/Host.hpp"
#include "Base/Threads/ReadLock.hpp"
#include "Base/Threads/WriteLock.hpp"
#include "Base/ViaPointer.hpp"
#include "Commons/ViaUnorderedCollection.hpp"
#include "Logger/Logger.hpp"

using std::stringstream;

namespace Persistency
{

namespace
{
template<typename T>
Base::NullValue<Host::Netmask> makeNetmask(const T *mask)
{
  if(mask!=NULL)
    return Base::NullValue<Host::Netmask>(*mask);
  return Base::NullValue<Host::Netmask>(NULL);
} // makeNetmask()
}

Host::Host(const IPv4            &ip,
           const Netmask_v4      *mask,
           const OperatingSystem  os,
           ReferenceURLPtr        url,
           const Services        &services,
           const Processes       &processes,
           const Name            &name):
  ip_(ip),
  mask_( makeNetmask(mask) ),
  os_(os),
  name_(name),
  url_(url),
  services_(services),
  processes_(processes)
{
}

Host::Host(const IPv6            &ip,
           const Netmask_v6      *mask,
           const OperatingSystem  os,
           ReferenceURLPtr        url,
           const Services        &services,
           const Processes       &processes,
           const Name            &name):
  ip_(ip),
  mask_( makeNetmask(mask) ),
  os_(os),
  name_(name),
  url_(url),
  services_(services),
  processes_(processes)
{
}

Host::Host(const IP              &ip,
           const Netmask         *mask,
           const OperatingSystem  os,
           ReferenceURLPtr        url,
           const Services        &services,
           const Processes       &processes,
           const Name            &name):
  ip_(ip),
  mask_(mask),
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

Host::Name Host::getName(void) const
{
  Base::Threads::ReadLock lock(mutex_);
  return name_;
}

const ReferenceURL *Host::getReferenceURL(void) const
{
  return url_.get();
}

const Host::Services &Host::getServices(void) const
{
  return services_;
}

const Host::Processes &Host::getProcesses(void) const
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
  if( !Commons::ViaUnorderedCollection::equal( getServices(), other.getServices() ) )
    return false;
  if( !Commons::ViaUnorderedCollection::equal( getProcesses(), other.getProcesses() ) )
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

  Base::Threads::WriteLock lock(mutex_);
  if( name_.get()!=NULL )
    throw ExceptionHostNameAlreadySet(SYSTEM_SAVE_LOCATION, name_.get() );

  // add new host entry
  name_=name;
  assert( name_.get()!=NULL );
}

} // namespace Persistency
