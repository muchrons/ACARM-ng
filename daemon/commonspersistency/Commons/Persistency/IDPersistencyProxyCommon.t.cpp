/*
 * IDPersistencyProxyCommon.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Transaction.hpp"
#include "Input/IDPersistencyProxyCommon.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( createUserStub() ),
    t_( conn_->createNewTransaction("idppc_test") ),
    idppc_(conn_, t_)
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
  IDPersistencyProxyCommon         idppc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/IDPersistencyProxyCommon");
} // unnamed namespace


namespace tut
{

// test getting paramter when it does not exist
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid deafult value", idppc_.readNextFreeID().get(), 0u);
}

// test getting paramter when it was already changed/set
template<>
template<>
void testObj::test<2>(void)
{
  idppc_.saveNextFreeID(31u);
  ensure_equals("invalid deafult value", idppc_.readNextFreeID().get(), 31u);
}

// test getting paramter when it already existst
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid value (create)", idppc_.readNextFreeID().get(), 0u);
  ensure_equals("invalid value (read)",   idppc_.readNextFreeID().get(), 0u);
}

} // namespace tut
