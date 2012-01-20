/*
 * MessageIO.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Jabber/MessageIO.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/delay.hpp"

using namespace Trigger::Jabber;
using namespace TestHelpers::Persistency;
using TestHelpers::delay;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( getTestConfig1() )
  {
  }

  std::string getMessageFromAccount2(void) const
  {
    return getMessageFromAccount( getTestConfig2(),
                                  getTestConfig1().getLogin() + "@" + getTestConfig1().getServer() );
  }

  std::string getMessageFromAccount3(void) const
  {
    return getMessageFromAccount( getTestConfig3(),
                                  getTestConfig2().getLogin() + "@" + getTestConfig2().getServer() );
  }

  Connection conn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/MessageIO");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MessageIO ms(conn_);
}

// test sending message from one account to another.
template<>
template<>
void testObj::test<2>(void)
{
  MessageIO         ms(conn_);
  const std::string msg("alice has a cat");
  ms.send( getTestConfig2().getLogin()+"@"+getTestConfig2().getServer() , msg );
  const std::string recv=getMessageFromAccount2();
  ensure_equals("invalid message received", recv, msg);
}

// test if messages are buffered on jabber server
template<>
template<>
void testObj::test<3>(void)
{
  const std::string msg("alice has a cat");
  // connect
  {
    Connection        lconn( getTestConfig2() );
    MessageIO         ms(lconn);
    // send message
    ms.send( getTestConfig3().getLogin()+"@"+getTestConfig3().getServer() , msg );
    // disconnect
  }
  // wait one second
  delay(1);
  // receive message
  const std::string recv=getMessageFromAccount3();
  ensure_equals("invalid message received", recv, msg);
}

// test sending UTF8 message
template<>
template<>
void testObj::test<4>(void)
{
  MessageIO         ms(conn_);
  const std::string msg("łączność UTF-8");
  ms.send( getTestConfig2().getLogin()+"@"+getTestConfig2().getServer() , msg );
  const std::string recv=getMessageFromAccount2();
  ensure_equals("invalid message received", recv, msg);
}

// test discarding messages (smoke test)
template<>
template<>
void testObj::test<5>(void)
{
  MessageIO ms(conn_);
  ms.discardIncommingMessages();
}

} // namespace tut
