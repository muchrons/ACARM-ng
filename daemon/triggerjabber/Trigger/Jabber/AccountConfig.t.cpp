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
};
// TODO: const AccountConfig ac("server.com", "JabberUser", "TheAnswer");
//       should ba TestClass' member, since it is used in every test case

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
  const AccountConfig ac("server.com", "JabberUser", "TheAnswer");
  ensure_equals("invalid user's login", ac.getLogin(), "JabberUser");
}

// test getting password
template<>
template<>
void testObj::test<2>(void)
{
  const AccountConfig ac("server.com", "JabberUser", "TheAnswer");
  ensure_equals("invalid password", ac.getPassword(), "TheAnswer");
}

// test getting server name
template<>
template<>
void testObj::test<3>(void)
{
  const AccountConfig ac("server.com", "JabberUser", "TheAnswer");
  ensure_equals("invalid password", ac.getServer(), "server.com");
}

// test copyability
template<>
template<>
void testObj::test<4>(void)
{
  const AccountConfig ac1("server.com", "JabberUser", "TheAnswer");
  const AccountConfig ac2(ac1);
  ensure_equals("server name does not match", ac2.getServer(), ac1.getServer() );
  ensure_equals("login does not match", ac2.getLogin(), ac1.getLogin() );
  ensure_equals("password does not match", ac2.getPassword(), ac1.getPassword() );
}

} // namespace tut
