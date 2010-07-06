/*
 * IDPersistencyProxyBase.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Logger/Logger.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Input/detail/IDPersistencyProxyBase.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input::detail;

namespace
{

struct TestProxy: public IDPersistencyProxyBase
{
  explicit TestProxy(Persistency::IO::ConnectionPtrNN  conn,
                     Persistency::IO::Transaction     &t):
    IDPersistencyProxyBase("TestOwner", conn, t)
  {
    LOGMSG_DEBUG(log_, "hello test! (compile-test for logger presence");
  }

  Persistency::IO::DynamicConfig &get(void)
  {
    return dynConf_;
  }
}; // struct TestProxy

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( Input::createUserStub() ),
    t_( conn_->createNewTransaction("idppc_test") ),
    tp_(conn_, t_)
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
  TestProxy                        tp_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/detail/IDPersistencyProxyBase");
} // unnamed namespace


namespace tut
{

// test doing some basin I/O
template<>
template<>
void testObj::test<1>(void)
{
  tp_.get().write("some key", "some value");
  Persistency::IO::DynamicConfig::ValueNULL v=tp_.get().read("some key");
  ensure("NULL read", v.get()!=NULL);
  ensure_equals("invalid value", v.get()->get(), std::string("some value") );
}

} // namespace tut
