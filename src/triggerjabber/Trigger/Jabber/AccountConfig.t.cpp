/*
 * AccountConfig.t.cpp
 *
 */
#include "Trigger/Jabber/AccountConfig.hpp"
#include <tut.h>

using namespace std;
using namespace Trigger::Jabber;

namespace
{

struct TestClass
{
  TestClass(void):
    ac_(AccountConfig("server.com", "JabberUser", "TheAnswer"))
  {
  }
  AccountConfig ac_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/AccountConfig");
} // unnamed namespace


namespace tut
{

// test getting user's login
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid user's login", ac_.getLogin(), "JabberUser");
}

// test getting password
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid password", ac_.getPassword(), "TheAnswer");
}

// test getting server name
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid password", ac_.getServer(), "server.com");
}

// test copyability
template<>
template<>
void testObj::test<4>(void)
{
  const AccountConfig ac(ac_);
  ensure_equals("server name does not match", ac.getServer(), ac_.getServer() );
  ensure_equals("login does not match", ac.getLogin(), ac_.getLogin() );
  ensure_equals("password does not match", ac.getPassword(), ac_.getPassword() );
}

} // namespace tut
