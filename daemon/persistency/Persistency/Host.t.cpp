/*
 * Host.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Host.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

const Host::Netmask_v4::bytes_type mask4_bytes={ { 0xFF, 0xFF, 0x00, 0x00 } };
const Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                   0xFF, 0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                               } };

struct TestClass: private TestBase
{
  TestClass(void):
    mask4_( Host::Netmask_v4(mask4_bytes) ),
    mask6_( Host::Netmask_v6(mask6_bytes) ),
    custom_(  Host::IPv4::from_string("1.2.3.4"),
             &mask4_,
              "myos",
              makeNewReferenceURL(),
              Host::ReportedServices(),
              Host::ReportedProcesses() )
  {
  }

  const Host::Netmask_v4 mask4_;
  const Host::Netmask_v6 mask6_;
  const Host             custom_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Host");
} // unnamed namespace


namespace tut
{

// test creating IPv4
template<>
template<>
void testObj::test<1>(void)
{
  const HostPtr ti=makeNewHost4("1.2.3.4", &mask4_, "myOS");
  ensure_equals("invalid IP",   ti->getIP().to_string(),        "1.2.3.4"      );
  ensure_equals("invalid mask", ti->getNetmask()->to_string(),  "255.255.0.0"  );
  ensure_equals("invalid OS",   ti->getOperatingSystem().get(), string("myOS") );
  ensure("invalid URL", ti->getReferenceURL()!=NULL);
}

// test creating IPv6
template<>
template<>
void testObj::test<2>(void)
{
  const HostPtr ti=makeNewHost6("::1.2.3.4", &mask6_, "myOS");
  ensure_equals("invalid IP",   ti->getIP().to_string(),        "::1.2.3.4"             );
  ensure_equals("invalid mask", ti->getNetmask()->to_string(),  "ffff:ffff:ffff:ff00::" );
  ensure_equals("invalid OS",   ti->getOperatingSystem().get(), string("myOS")          );
  ensure("invalid URL", ti->getReferenceURL()!=NULL);
}

// test NULL mask for IPv4
template<>
template<>
void testObj::test<3>(void)
{
  const HostPtr ti=makeNewHost4("1.2.3.4", NULL, "myOS");
  ensure("invalid mask", ti->getNetmask()==NULL);
}

// test NULL mask for IPv6
template<>
template<>
void testObj::test<4>(void)
{
  const HostPtr ti=makeNewHost6("::1.2.3.4", NULL, "myOS");
  ensure("invalid mask", ti->getNetmask()==NULL);
}

// test NULL URL for IPv4
template<>
template<>
void testObj::test<5>(void)
{
  const HostPtr ti=makeNewHost4("1.2.3.4", &mask4_, "myOS", true);
  ensure("invalid URL", ti->getReferenceURL()==NULL);
}

// test NULL mask for IPv6
template<>
template<>
void testObj::test<6>(void)
{
  const HostPtr ti=makeNewHost6("::1.2.3.4", &mask6_, "myOS", true);
  ensure("invalid URL", ti->getReferenceURL()==NULL);
}

// test equality for different IP
template<>
template<>
void testObj::test<7>(void)
{
  const Host h(  Host::IPv4::from_string("1.6.6.6"),
                &mask4_,
                 "myos",
                 makeNewReferenceURL(),
                 Host::ReportedServices(),
                 Host::ReportedProcesses() );
  TestHelpers::checkEquality(custom_, h);
}

// test equality for different mask
template<>
template<>
void testObj::test<8>(void)
{
  const Host::Netmask_v4::bytes_type mask4_bytes={ { 0xFF, 0xFF, 0xFF, 0x00 } };
  const Host::Netmask_v4             msk(mask4_bytes);
  const Host h(  Host::IPv4::from_string("1.2.3.4"),
                &msk,
                 "myos",
                 makeNewReferenceURL(),
                 Host::ReportedServices(),
                 Host::ReportedProcesses() );
  TestHelpers::checkEquality(custom_, h);
}

// test equality for different reference URL
template<>
template<>
void testObj::test<9>(void)
{
  const Host h(  Host::IPv4::from_string("1.2.3.4"),
                &mask4_,
                 "myos",
                 ReferenceURLPtr(),
                 Host::ReportedServices(),
                 Host::ReportedProcesses() );
  TestHelpers::checkEquality(custom_, h);
}

// test equality for different reported services
template<>
template<>
void testObj::test<10>(void)
{
  Host::ReportedServices rs;
  ServicePtrNN           serv( new Service("name", 42, "proto", ReferenceURLPtr() ) );
  rs.push_back(serv);
  const Host h(  Host::IPv4::from_string("1.2.3.4"),
                &mask4_,
                 "myos",
                 makeNewReferenceURL(),
                 rs,
                 Host::ReportedProcesses() );
  TestHelpers::checkEquality(custom_, h);
}

// test equality for different reported processes
template<>
template<>
void testObj::test<11>(void)
{
  Host::ReportedProcesses rp;
  rp.push_back( makeNewProcess() );
  const Host h(  Host::IPv4::from_string("1.2.3.4"),
                &mask4_,
                 "myos",
                 makeNewReferenceURL(),
                 Host::ReportedServices(),
                 rp );
  TestHelpers::checkEquality(custom_, h);
}

// test equality for different object representing the same values
template<>
template<>
void testObj::test<12>(void)
{
  Host::ReportedProcesses rp1;
  rp1.push_back( makeNewProcess() );
  const Host h1(  Host::IPv4::from_string("1.2.3.4"),
                 &mask4_,
                  "myos",
                  makeNewReferenceURL(),
                  Host::ReportedServices(),
                  rp1 );

  Host::ReportedProcesses rp2;
  rp2.push_back( makeNewProcess() );
  const Host h2(  Host::IPv4::from_string("1.2.3.4"),
                 &mask4_,
                  "myos",
                  makeNewReferenceURL(),
                  Host::ReportedServices(),
                  rp2 );

  TestHelpers::checkEquality(h1, h2, custom_);
}

} // namespace tut
