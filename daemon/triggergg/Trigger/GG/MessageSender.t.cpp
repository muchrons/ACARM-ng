/*
 * MessageSender.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/MessageSender.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn1_( getTestConfig1() )
  {
  }

  std::string getMessageFromAccount2(void) const
  {
    return getMessageFromAccount( getTestConfig2(), getTestConfig1().getUserID() );
  }

  Connection conn1_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/MessageSender");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MessageSender ms(conn1_);
}

// test sending message from one account to another.
template<>
template<>
void testObj::test<2>(void)
{
  MessageSender     ms(conn1_);
  const std::string msg("alice has a cat");
  ms.send( getTestConfig2().getUserID(), msg );
  const std::string recv=getMessageFromAccount2();
  ensure_equals("invalid message received", recv, msg);
}

// test sending UTF8 message
template<>
template<>
void testObj::test<3>(void)
{
  MessageSender     ms(conn1_);
  const std::string msg("łączność UTF-8");
  ms.send( getTestConfig2().getUserID(), msg );
  const std::string recv=getMessageFromAccount2();
  ensure_equals("invalid message received", recv, msg);
}

} // namespace tut
