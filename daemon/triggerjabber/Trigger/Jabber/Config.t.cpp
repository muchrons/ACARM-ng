/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Jabber/Config.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Jabber;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    cfg_( AccountConfig("server.com", "user", "abc"),
          Config::Receivers("receiver@server.com"),
          Trigger::Simple::ThresholdConfig("1.2", "4") )
  {
  }

  const Config cfg_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/Config");
} // unnamed namespace


namespace tut
{

// test getting account
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid server name", cfg_.getAccountConfig().getServer(), "server.com");
  ensure_equals("invalid account's login", cfg_.getAccountConfig().getLogin(), "user");
  ensure_equals("invalid account's password", cfg_.getAccountConfig().getPassword(), "abc");
}

// test getting receiver's name
template<>
template<>
void testObj::test<2>(void)
{
  Config::Receivers::const_iterator it=cfg_.getReceivers().begin();
  ensure("no elements saved", it!=cfg_.getReceivers().end() );
  ensure_equals("invalid receiver's name", *it, "receiver@server.com");
}

// get severity threshold
template<>
template<>
void testObj::test<3>(void)
{
  ensure("severity threshold is NULL", cfg_.getThresholdConfig().getSeverityThreshold().get()!=NULL );
  ensure_equals("invalid severity threshold", *cfg_.getThresholdConfig().getSeverityThreshold().get(), 1.2);

  ensure("alerts' count threshold is NULL", cfg_.getThresholdConfig().getAlertCountThreshold().get()!=NULL );
  ensure_equals("invalid alerts' count threshold", *cfg_.getThresholdConfig().getAlertCountThreshold().get(), 4);
}

// test copyability
template<>
template<>
void testObj::test<4>(void)
{
  const Config tmp=cfg_;
}

// test multiple receivers
template<>
template<>
void testObj::test<5>(void)
{
  Config::Receivers r("receiver1@jabber.org");
  r.push_back("receiver2@jabber.org");
  const Config c(AccountConfig("server.com", "user", "abc"), r, Trigger::Simple::ThresholdConfig("1.2", "4") );

  ensure_equals("invalid receivers count", c.getReceivers().size(), 2u);
  Config::Receivers::const_iterator it=c.getReceivers().begin();

  ensure("no elements?", it!=c.getReceivers().end() );
  ensure_equals("invalid receiver 1", *it, "receiver1@jabber.org");
  ++it;

  ensure("only one element?", it!=c.getReceivers().end() );
  ensure_equals("invalid receiver 2", *it, "receiver2@jabber.org");
  ++it;

  ensure("too many elements?", it==c.getReceivers().end() );
}
} // namespace tut
