/*
 * StrategyIO.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "Trigger/GG/detail/StrategyIO.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace Trigger::GG::detail;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( getTestConfig3(),
          Config::Receivers( getTestConfig4().getUserID() ),
          Trigger::Simple::ThresholdConfig("1.2", "2") ),
    io_(cfg_)
  {
  }

  const Config cfg_;
  StrategyIO   io_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/detail/StrategyIO");
} // unnamed namespace


namespace tut
{

// smoke test for ping()
template<>
template<>
void testObj::test<1>(void)
{
  io_.ping();
}

// test sending message
template<>
template<>
void testObj::test<2>(void)
{
  io_.send("hello world");
  const std::string str=getMessageFromAccount( getTestConfig4(), cfg_.getAccountConfig().getUserID() );
  ensure_equals("invalid repot generated", str, "hello world");
}

// test discarding all incomming messages - smoke test
template<>
template<>
void testObj::test<3>(void)
{
  io_.discardIncommingMessages();
}

// test sending messages to multiple receivers
template<>
template<>
void testObj::test<4>(void)
{
  // sending the message
  Config::Receivers r( getTestConfig4().getUserID() );
  r.push_back( getTestConfig2().getUserID() );
  Config            cfg( getTestConfig1(), r, Trigger::Simple::ThresholdConfig("1.2", "2") );
  StrategyIO        io(cfg);
  io.send("hello world");

  // get from account 4
  {
    const std::string str=getMessageFromAccount( getTestConfig4(), cfg.getAccountConfig().getUserID(), "OP 2" );
    ensure_equals("invalid repot generated on account 4", str, "hello world");
  }
  // get from account 2
  {
    const std::string str=getMessageFromAccount( getTestConfig2(), cfg.getAccountConfig().getUserID(), "OP 1" );
    ensure_equals("invalid repot generated on account 2", str, "hello world");
  }
}

} // namespace tut
