/*
 * OnService.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnService.hpp"
#include "Persistency/Facades/StrAccess/TestBaseData.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass: public TestBaseData<OnService>
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Facades/StrAccess/OnService");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensureProc("invalid name", *service_, "service.name", "servicename");
}

// test getting port number
template<>
template<>
void testObj::test<2>(void)
{
  ensureProc("invalid port", *service_, "service.port", "42");
}

// test processing last element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("service.name"), cb_);
  ++p;
  ensureThrow(*service_, p);
}

// test processing for end element
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("service.name"), cb_);
  ++p;
  ++p;
  ensureThrow(*service_, p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("service.enemyunknown"), cb_);
  ensureThrow(*service_, p);
}

// test error when invalid root is requested
template<>
template<>
void testObj::test<6>(void)
{
  TestParams p(Path("whatisthat.name"), cb_);
  ensureThrow(*service_, p);
}

// test getting protocol
template<>
template<>
void testObj::test<7>(void)
{
  ensureProc("invalid protocol", *service_, "service.protocol", "serviceproto");
}

// test getting reference url
template<>
template<>
void testObj::test<8>(void)
{
  ensureProc("invalid referenceurl->name", *service_, "service.referenceurl.name", "evil site");
}

// test when reference url is NULL
template<>
template<>
void testObj::test<9>(void)
{
  ensureProcNull("invalid null handle on reference url", *serviceNull_, "service.referenceurl.name", "referenceurl");
}

// test when protocol is NULL
template<>
template<>
void testObj::test<10>(void)
{
  ensureProcNull("invalid protocol", *serviceNull_, "service.protocol", "protocol");
}

} // namespace tut
