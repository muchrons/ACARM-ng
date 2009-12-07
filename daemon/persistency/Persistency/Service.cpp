/*
 * Service.cpp
 *
 */

#include "Persistency/Service.hpp"

namespace Persistency
{

const Service::Name &Service::getName(void) const
{
  return name_;
}

Service::Port Service::getPort(void) const
{
  return port_;
}

const Service::Protocol Service::getProtocol(void) const
{
  return protocol_;
}

const ReferenceURL *Service::getReferenceURL(void) const
{
  return url_.get();
}

Service::Service(const Name      &name,
                 const Port      &port,
                 const Protocol  &protocol,
                 ReferenceURLPtr  url):
  name_(name),
  port_(port),
  protocol_(protocol),
  url_(url)
{
}

} // namespace Persistency
