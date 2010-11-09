/*
 * StrategyIO.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Trigger/Jabber/detail/StrategyIO.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Jabber;
using namespace Trigger::Jabber::detail;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( getTestConfig1(),
          Config::Receivers( getTestConfig2().getLogin() + "@" + getTestConfig2().getServer() ),
          Trigger::Simple::ThresholdConfig("1.2", "2") ),
    io_(cfg_)
  {
  }

  const Config cfg_;
  StrategyIO   io_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/detail/StrategyIO");
} // unnamed namespace


namespace tut
{

// test sending message
template<>
template<>
void testObj::test<1>(void)
{
  io_.send("hello world");
  const std::string str=getMessageFromAccount( getTestConfig2(), cfg_.getAccountConfig().getLogin() + "@" + cfg_.getAccountConfig().getServer());
  ensure_equals("invalid repot generated", str, "hello world");
}

// test discarding all incomming messages - smoke test
template<>
template<>
void testObj::test<2>(void)
{
  io_.discardIncommingMessages();
}

} // namespace tut
