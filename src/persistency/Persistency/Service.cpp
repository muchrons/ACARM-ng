/*
 * Service.cpp
 *
 */
#include "Base/ViaPointer.hpp"
#include "Persistency/Service.hpp"

namespace Persistency
{

Service::Service(const Name           &name,
                 const PortNumber     &port,
                 const Protocol       &protocol,
                 ConstReferenceURLPtr  url):
  name_(name),
  port_(port),
  protocol_(protocol),
  url_(url)
{
}

const Service::Name &Service::getName(void) const
{
  return name_;
}

PortNumber Service::getPort(void) const
{
  return port_;
}

const Service::Protocol &Service::getProtocol(void) const
{
  return protocol_;
}

ConstReferenceURLPtr Service::getReferenceURL(void) const
{
  return url_;
}

bool Service::operator==(const Service &other) const
{
  if(this==&other)
    return true;

  if( getName()!=other.getName() )
    return false;
  if( getPort()!=other.getPort() )
    return false;
  if( getProtocol()!=other.getProtocol() )
    return false;
  if( !Base::ViaPointer::equal( getReferenceURL().get(), other.getReferenceURL().get() ) )
    return false;
  // ok
  return true;
}

} // namespace Persistency
