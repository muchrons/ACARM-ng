/*
 * MessageIO.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/MessageIO.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn3_( getTestConfig3() ),
    conn4_( getTestConfig4() )
  {
  }

  std::string getMessageFromAccount4(void)
  {
    return getMessageFromAccount(conn4_, getTestConfig3().getUserID() );
  }

  Connection conn3_;
  Connection conn4_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/MessageIO");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MessageIO ms(conn3_);
}

// test sending message from one account to another.
template<>
template<>
void testObj::test<2>(void)
{
  cleanUpMessagesFromAccount(conn4_);
  MessageIO         ms(conn3_);
  const std::string msg("alice has a cat");
  ms.send( getTestConfig4().getUserID(), msg );
  const std::string recv=getMessageFromAccount4();
  ensure_equals("invalid message received", recv, msg);
}

// test sending UTF8 message
template<>
template<>
void testObj::test<3>(void)
{
  cleanUpMessagesFromAccount(conn4_);
  MessageIO         ms(conn3_);
  const std::string msg("łączność UTF-8");
  ms.send( getTestConfig4().getUserID(), msg );
  const std::string recv=getMessageFromAccount4();
  ensure_equals("invalid message received", recv, msg);
}

// test discarding messages (smoke test)
template<>
template<>
void testObj::test<4>(void)
{
  MessageIO ms(conn3_);
  ms.discardIncommingMessages();
}

} // namespace tut
