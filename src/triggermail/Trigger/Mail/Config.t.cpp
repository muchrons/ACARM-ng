/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Mail/Config.hpp"
#include "Trigger/Mail/Config.hpp"

using namespace Trigger::Mail;

namespace
{

struct TestClass
{
  TestClass(void):
    th_("1", "2"),
    srv_("server",
          1234,
          ::Mail::Config::Server::Protocol::SMTP,
          ::Mail::Config::Server::Security::SSL, "/cert/path"),
    auth_("john", "doe"),
    to_("to")
  {
  }

  const Trigger::Simple::ThresholdConfig th_;
  const ::Mail::Config::Server             srv_;
  const ::Mail::Config::Authorization      auth_;
  const ::Mail::Config::Recipients         to_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/Config");
} // unnamed namespace


namespace tut
{

// test creating config w-out authorization
template<>
template<>
void testObj::test<1>(void)
{
  const Config c(th_, ::Mail::Config("from", srv_), to_);
  ensure("authorization required", c.getMailConfig().getAuthorizationConfig()==NULL );
}

// test creating config with authorization
template<>
template<>
void testObj::test<2>(void)
{
  const Config c(th_, ::Mail::Config("from", srv_, auth_), to_);
  ensure("authorization not required", c.getMailConfig().getAuthorizationConfig()!=NULL );
}

// test required parameters c-tor
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid server address", srv_.server_, "server");
  ensure_equals("invalid port number", srv_.port_, 1234u);
  ensure("invalid TLS setting", srv_.sec_==Mail::Config::Server::Security::SSL);
  ensure("invalid TLS setting", srv_.proto_==Mail::Config::Server::Protocol::SMTP);
  ensure_equals("invalid cert path", srv_.rootCAcertPath_, "/cert/path");
}

// test authorization config
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid user name", auth_.user_, "john");
  ensure_equals("invalid password", auth_.pass_, "doe");
}

// test recipients addresses list
template<>
template<>
void testObj::test<5>(void)
{
  const Config c(th_, ::Mail::Config("from", srv_, auth_), to_);
  ensure_equals("invalid from address", c.getMailConfig().getSenderAddress(), "from");
  ensure_equals("invalid number of recipients", c.getRecipientsAddresses().size(), 1u);
  ensure_equals("invalid recipient", c.getRecipientsAddresses()[0], "to");
}

} // namespace tut
