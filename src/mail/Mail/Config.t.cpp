/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Mail/Config.hpp"

using namespace Mail;

namespace
{

struct TestClass
{
  TestClass(void):
    srv_("server", 1234, Config::Server::Protocol::SMTP, Config::Server::Security::SSL, "/cert/path"),
    auth_("john", "doe")
  {
  }

  const Config::Server                   srv_;
  const Config::Authorization            auth_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Mail/Config");
} // unnamed namespace


namespace tut
{

// test creating config w-out authorization
template<>
template<>
void testObj::test<1>(void)
{
  const Config c("from", srv_);
  ensure("authorization required", c.getAuthorizationConfig()==NULL );
}

// test creating config with authorization
template<>
template<>
void testObj::test<2>(void)
{
  const Config c("from", srv_, auth_);
  ensure("authorization not required", c.getAuthorizationConfig()!=NULL );
}

// test required parameters c-tor
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid server address", srv_.server_, "server");
  ensure_equals("invalid port number", srv_.port_, 1234u);
  ensure("invalid TLS setting", srv_.sec_==Config::Server::Security::SSL);
  ensure("invalid TLS setting", srv_.proto_==Config::Server::Protocol::SMTP);
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


} // namespace tut
