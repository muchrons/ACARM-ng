/*
 * Host.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Host.hpp"
#include "Persistency/Host.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

const Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                   0xFF, 0xFF, 0xFF, 0xFF,
                                                   0xFF, 0xFF, 0xFF, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                                   0x00, 0x00, 0x00, 0x00,
                                               } };

struct HostTestClass
{
  HostTestClass(void):
    mask4_( Host::Netmask_v4(mask4_bytes) ),
    mask6_( Host::Netmask_v6(mask6_bytes) )
  {
  }

  const Host::Netmask_v4 mask4_;
  const Host::Netmask_v6 mask6_;
};

typedef HostTestClass TestClass;
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
  const TestImpl ti("1.2.3.4", &mask4_, "myOS");
  ensure_equals("invalid IP",   ti.getIP().to_string(),        "1.2.3.4"      );
  ensure_equals("invalid mask", ti.getNetmask()->to_string(),  "255.255.0.0"  );
  ensure_equals("invalid OS",   ti.getOperatingSystem().get(), string("myOS") );
}

// test creating IPv6
template<>
template<>
void testObj::test<2>(void)
{
  const TestImpl ti("::1.2.3.4", &mask6_, "myOS");
  ensure_equals("invalid IP",   ti.getIP().to_string(),        "::1.2.3.4"             );
  ensure_equals("invalid mask", ti.getNetmask()->to_string(),  "ffff:ffff:ffff:ff00::" );
  ensure_equals("invalid OS",   ti.getOperatingSystem().get(), string("myOS")          );
}

// test NULL mask for IPv4
template<>
template<>
void testObj::test<3>(void)
{
  const TestImpl ti("1.2.3.4", static_cast<Host::Netmask_v4*>(NULL), "myOS");
  ensure("invalid mask", ti.getNetmask()==NULL);
}

// test NULL mask for IPv6
template<>
template<>
void testObj::test<4>(void)
{
  const TestImpl ti("::1.2.3.4", static_cast<Host::Netmask_v6*>(NULL), "myOS");
  ensure("invalid mask", ti.getNetmask()==NULL);
}

} // namespace tut
