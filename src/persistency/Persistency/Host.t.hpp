/*
 * Host.t.cpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_HOST_T_HPP_FILE
#define INCLUDE_PERSISTENCY_HOST_T_HPP_FILE

#include "Persistency/Host.hpp"

namespace
{

struct TestImpl: public Persistency::Host
{
private:
  typedef Persistency::Host Host;
public:

  TestImpl(const char *ip, const Host::Netmask_v4 *mask, const char *os):
    Host( Host::IPv4::from_string(ip),
          mask,
          os )
  {
  }

  TestImpl(const char *ip, const Host::Netmask_v6 *mask, const char *os):
    Host( Host::IPv6::from_string(ip),
          mask,
          os )
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
    return Persistency::HostPtr( new TestImpl("1.2.3.4", null, os) );
  }
}; // struct TestImpl

} // unnamed namespace

#endif
