/*
 * OnService.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Facades/StrAccess/OnService.hpp"
#include "Persistency/Facades/StrAccess/TestParams.t.hpp"

using namespace Persistency;
using namespace Persistency::Facades::StrAccess;

namespace
{
struct TestClass
{
  TestClass(void):
    r_( new ReferenceURL("somename", "http://evil.one") ),
    s_("servicename", 42, "serviceproto", r_),
    sn_("servicename", 42, Service::Protocol(NULL), ReferenceURLPtr() )
  {
  }

  void ensureThrow(TestParams &p)
  {
    try
    {
      OnService::process(s_, p);
      tut::fail("call didn't throw on invalid path");
    }
    catch(const ExceptionInvalidPath &)
    {
      // this is expected
    }
  }

  TestParams::ResultCallback cb_;
  ReferenceURLPtr            r_;
  Service                    s_;
  Service                    sn_;
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
  TestParams p(Path("service.name"), cb_);
  OnService::process(s_, p);
  ensure_equals("invalid name", cb_.lastValue_, "servicename");
}

// test getting port number
template<>
template<>
void testObj::test<2>(void)
{
  TestParams p(Path("service.port"), cb_);
  OnService::process(s_, p);
  ensure_equals("invalid port", cb_.lastValue_, "42");
}

// test processing last element
template<>
template<>
void testObj::test<3>(void)
{
  TestParams p(Path("service.name"), cb_);
  ++p;
  ensureThrow(p);
}

// test processing for end element
template<>
template<>
void testObj::test<4>(void)
{
  TestParams p(Path("service.name"), cb_);
  ++p;
  ++p;
  ensureThrow(p);
}

// test processing for unknown name
template<>
template<>
void testObj::test<5>(void)
{
  TestParams p(Path("service.enemyunknown"), cb_);
  ensureThrow(p);
}

// test error when invalid root is requested
template<>
template<>
void testObj::test<6>(void)
{
  TestParams p(Path("whatisthat.name"), cb_);
  ensureThrow(p);
}

// test getting protocol
template<>
template<>
void testObj::test<7>(void)
{
  TestParams p(Path("service.protocol"), cb_);
  OnService::process(s_, p);
  ensure_equals("invalid protocol", cb_.lastValue_, "serviceproto");
}

// test getting reference url
template<>
template<>
void testObj::test<8>(void)
{
  TestParams p(Path("service.referenceurl.name"), cb_);
  OnService::process(s_, p);
  ensure_equals("invalid referenceurl/name", cb_.lastValue_, "somename");
}

// test when reference url is NULL
template<>
template<>
void testObj::test<9>(void)
{
  TestParams p(Path("service.referenceurl.name"), cb_);
  OnService::process(sn_, p);
  ensure_equals("invalid null handle on reference url", cb_.lastNullFound_, "referenceurl");
}

// test when protocol is NULL
template<>
template<>
void testObj::test<10>(void)
{
  TestParams p(Path("service.protocol"), cb_);
  OnService::process(sn_, p);
  ensure_equals("invalid null handle for protocol", cb_.lastNullFound_, "protocol");
}

} // namespace tut
