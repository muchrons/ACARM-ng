/*
 * OnHost.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnHost.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnHost>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnHost");
} // unnamed namespace


namespace tut
{

// test hosting last element
template<>
template<>
void testObj::test<1>(void)
{
  TestParams p(Path("host.name"), cb_);
  ++p;
  ensureThrow(*host_, p);
}

// test hosting for end element
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("host.name"), cb_);
  ++p;
  ++p;
  ensureThrow(*host_, p);
}

// test hosting for unknown name
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("host.enemyunknown"), cb_);
  ensureThrow(*host_, p);
}

// test accessing via collection index
template<>
template<>
void testObj::test<4>(void)
{
  ensureProc("failed for colleciton index", *host_, "42.name", "my.dns.name");
}

// test getting ip
template<>
template<>
void testObj::test<5>(void)
{
  ensureProc("invalid ip", *host_, "host.ip", "1.2.3.4");
}

// test getting ip when NULL
template<>
template<>
void testObj::test<6>(void)
{
  ensureProcNull("no NULL on NULL ip", *hostNull_, "host.netmask", "netmask");
}

// test getting os name
template<>
template<>
void testObj::test<7>(void)
{
  ensureProc("invalid os name", *host_, "host.os", "Linux");
}

// test getting os name when NULL
template<>
template<>
void testObj::test<8>(void)
{
  ensureProcNull("no NULL on NULL os name", *hostNull_, "host.os", "os");
}

// test getting reference url
template<>
template<>
void testObj::test<9>(void)
{
  ensureProc("invalid url", *host_, "host.referenceurl.url", "http://evil.one");
}

// test getting reference url when NULL
template<>
template<>
void testObj::test<10>(void)
{
  ensureProcNull("no NULL on NULL ref. url", *hostNull_, "host.referenceurl.url", "referenceurl");
}

// test services collection size
template<>
template<>
void testObj::test<11>(void)
{
  ensureProcSize("invalid collection size", *host_, "host.services.size", 2);
}

// test services first element access
template<>
template<>
void testObj::test<12>(void)
{
  ensureProc("invalid collection element 0", *host_, "host.services.0.name", "servicename");
}

// test services last element access
template<>
template<>
void testObj::test<13>(void)
{
  ensureProc("invalid collection element 1", *host_, "host.services.1.name", "otherservice");
}

// test accessing out-of-band service
template<>
template<>
void testObj::test<14>(void)
{
  TestParams p(Path("host.services.2.name"), cb_);
  ensureThrow(*host_, p);
}

// test getting processes collection size
template<>
template<>
void testObj::test<15>(void)
{
  ensureProcSize("invalid colleciton size", *host_, "host.processes.size", 1);
}

// test getting processes collection element
template<>
template<>
void testObj::test<16>(void)
{
  ensureProc("invalid collection element 0", *host_, "host.processes.0.name", "somename");
}

// test getting dns name
template<>
template<>
void testObj::test<17>(void)
{
  ensureProc("invalid dns name", *host_, "host.name", "my.dns.name");
}

// test getting dns name when NULL
template<>
template<>
void testObj::test<18>(void)
{
  ensureProcNull("no NULL on NULL dns name", *hostNull_, "host.name", "name");
}

// test exception on invalid root name
template<>
template<>
void testObj::test<19>(void)
{
  TestParams p(Path("invalidhostname.name"), cb_);
  ensureThrow(*host_, p);
}

} // namespace tut
