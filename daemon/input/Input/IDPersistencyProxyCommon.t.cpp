/*
 * IDPersistencyProxyCommon.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/create.hpp"
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
  ensure_equals("invalid deafult value", idppc_.readNextFreeID(), 0u);
}

// 
template<>
template<>
void testObj::test<2>(void)
{
}

} // namespace tut
