/*
 * Service.cpp
 *
 */

#include "Persistency/Service.hpp"

namespace Persistency
{

Service::~Service(void)
{
}

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

Service::Service(const Name     &name,
                 const Port     &port,
                 const Protocol &protocol):
  name_(name),
  port_(port),
  protocol_(protocol)
{
}

} // namespace Persistency
