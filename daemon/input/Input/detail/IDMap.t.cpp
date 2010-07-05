/*
 * IDMap.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "Persistency/IO/Transaction.hpp"
#include "Input/detail/IDMap.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Input;
using namespace Input::detail;

namespace
{

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( Input::createUserStub() ),
    t_( conn_->createNewTransaction("idppc_test") ),
    ppc_(conn_, t_),
    pps_(conn_, t_, "some owner")
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  Persistency::IO::Transaction     t_;
  IDPersistencyProxyCommon         ppc_;
  IDPersistencyProxySpecific       pps_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/detail/IDMap");
} // unnamed namespace


namespace tut
{

// test getting new ID
template<>
template<>
void testObj::test<1>(void)
{
  IDMap m(666);
  ensure_equals("invalid ID returned", m.get(ppc_, pps_, "some new entry"), 666u);
}

// test getting new ID multiple times
template<>
template<>
void testObj::test<2>(void)
{
  IDMap m(666);
  ensure_equals("invalid ID1 returned", m.get(ppc_, pps_, "some new entry 1"), 666u);
  ensure_equals("invalid ID2 returned", m.get(ppc_, pps_, "some new entry 2"), 667u);
}

// test getting ID for existing entry
template<>
template<>
void testObj::test<3>(void)
{
  IDMap m(666);
  ensure_equals("invalid ID1 returned (call 1)", m.get(ppc_, pps_, "some new entry 1"), 666u);
  ensure_equals("invalid ID2 returned (call 1)", m.get(ppc_, pps_, "some new entry 2"), 667u);
  ensure_equals("invalid ID1 returned (call 2)", m.get(ppc_, pps_, "some new entry 1"), 666u);
}

} // namespace tut
