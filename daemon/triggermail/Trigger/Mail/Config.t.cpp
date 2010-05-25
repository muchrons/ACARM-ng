/*
 * Config.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Mail/Config.hpp"

using namespace Trigger::Mail;

namespace
{

struct TestClass
{
  TestClass(void):
    th_("1", "2"),
    srv_("from", "server", 1234, true),
    auth_("john", "doe"),
    to_("to")
  {
  }

  const Trigger::Simple::ThresholdConfig th_;
  const Config::Server                   srv_;
  const Config::Authorization            auth_;
  const std::string                      to_;
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
  const Config c(th_, to_, srv_);
  ensure("authorization required", c.getAuthorizationConfig()==NULL );
}

// test creating config with authorization
template<>
template<>
void testObj::test<2>(void)
{
  const Config c(th_, to_, srv_, auth_);
  ensure("authorization not required", c.getAuthorizationConfig()!=NULL );
}

// test required parameters c-tor
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid from address", srv_.from_, "from");
  ensure_equals("invalid server address", srv_.server_, "server");
  ensure_equals("invalid port number", srv_.port_, 1234u);
  ensure_equals("invalid TLS setting", srv_.useTLS_, true);
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
