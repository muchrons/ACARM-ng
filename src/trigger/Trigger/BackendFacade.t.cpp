/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/BackendFacade.hpp"
#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"
#include "TestHelpers/Persistency/IOMetaAlertStub.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Trigger;
using namespace Persistency;
using namespace Core::Types::Proc;

namespace
{
struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( IO::create() ),
    ma_( TestHelpers::Persistency::makeNewMetaAlert() )
  {
    assert( conn_.get()!=NULL );
  }

  IO::ConnectionPtrNN conn_;
  MetaAlertPtrNN      ma_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/BackendFacade");
} // unnamed namespace


namespace tut
{

// smoke-test for marking as triggered
template<>
template<>
void testObj::test<1>(void)
{
  BackendFacade bf(conn_, TypeName("sometest"), InstanceName("myname"));
  bf.markAsTriggered(ma_);
}

namespace
{
// stub testing validity of the triggered name
struct IOMetaAlertTriggeredStub: public TestHelpers::Persistency::IOMetaAlertStub
{
  IOMetaAlertTriggeredStub(MetaAlertPtrNN   ma,
                           IO::Transaction &t):
    TestHelpers::Persistency::IOMetaAlertStub(ma, t)
  {
  }

  virtual void markAsTriggeredImpl(::Persistency::IO::Transaction &/*t*/, const std::string &name)
  {
    ensure_equals("invalid name of triggered meta-alert", name, "sometest_myname");
  }
}; // struct IOMetaAlertTriggeredStub

// connection producing IOMetaAlertTriggeredStub
struct ConnMAStub: public TestHelpers::Persistency::ConnectionUserStubBase
{
  explicit ConnMAStub(MetaAlertPtrNN ma):
    ma_(ma)
  {
  }

  virtual IO::MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, IO::Transaction &t)
  {
    ensure("invalid meta alert", ma.get()==ma_.get() );
    return IO::MetaAlertAutoPtr( new IOMetaAlertTriggeredStub(ma, t) );
  }

private:
  MetaAlertPtrNN ma_;
}; // struct TestHelpers::Persistency::ConnectionUserStubBase
} // unnamed namespace

// test if name is reported correctly
template<>
template<>
void testObj::test<2>(void)
{
  conn_=IO::ConnectionPtrNN( new ConnMAStub(ma_) );
  BackendFacade bf(conn_, TypeName("sometest"), InstanceName("myname"));
  bf.markAsTriggered(ma_);
}

} // namespace tut
