/*
 * IDPersistencyProxySpecific.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/IO/Transaction.hpp"
#include "Input/IDPersistencyProxySpecific.hpp"
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
    pps_(conn_, t_, "test'em all owner")
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
  IDPersistencyProxySpecific       pps_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/IDPersistencyProxySpecific");
} // unnamed namespace


namespace tut
{

// test adding new entry
template<>
template<>
void testObj::test<1>(void)
{
  pps_.saveMapping("some input", 42);
  TestConnection        &testConn=dynamic_cast<TestConnection&>(*conn_);            // to make references shorter
  ensure("data is NULL", testConn.data_.get()!=NULL );                              // sanity check...
  TestDynamicConfigStub  helper("Input::test'em all owner", t_, testConn.data_);    // create object's instance with the same
                                                                                    // data os pps_'s instance.
  ensure("NULL reposnse", helper.read("some input").get()!=NULL );
  ensure_equals("invalid ID saved", helper.read("some input").get()->get(), string("42") );
}

} // namespace tut
