/*
 * Host.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_HOST_T_HPP_FILE
#define INCLUDE_PERSISTENCY_HOST_T_HPP_FILE

#include "Persistency/Host.hpp"
#include "Persistency/ReferenceURL.t.hpp"

namespace
{

struct HostTestImpl: public Persistency::Host
{
private:
  typedef Persistency::Host            Host;
  typedef Persistency::ReferenceURLPtr ReferenceURLPtr;
public:

  HostTestImpl(const char *ip, const Host::Netmask_v4 *mask, const char *os, bool nullRef=false):
    Host( Host::IPv4::from_string(ip),
          mask,
          os,
          (nullRef)?(ReferenceURLPtr()):(ReferenceURLTestImpl::makeNew()),
          ReportedServices(),
          ReportedProcesses() )
  {
  }

  HostTestImpl(const char *ip, const Host::Netmask_v6 *mask, const char *os, bool nullRef=false):
    Host( Host::IPv6::from_string(ip),
          mask,
          os,
          (nullRef)?(ReferenceURLPtr()):(ReferenceURLTestImpl::makeNew()),
          ReportedServices(),
          ReportedProcesses() )
  {
  }

  virtual Name getName(void) const
  {
    return Name("some name");
  }

  virtual void setName(const Name &/*name*/)
  {
  }

  static Persistency::HostPtr makeNew(const char *os="linux")
  {
    const Host::Netmask_v4 *null=NULL;
    return Persistency::HostPtr( new HostTestImpl("1.2.3.4", null, os) );
  }
}; // struct TestImpl

} // unnamed namespace

#endif
