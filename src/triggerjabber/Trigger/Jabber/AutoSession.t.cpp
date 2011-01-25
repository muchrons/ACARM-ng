/*
 * AutoSession.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Jabber/Gloox.hpp"
#include "Trigger/Jabber/AutoSession.hpp"
#include "Trigger/Jabber/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Jabber;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Jabber/AutoSession");
} // unnamed namespace


namespace tut
{

// test default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const AutoSession ags;
  ensure("NULL not created by default", ags.get()==NULL );
}

// test some connection
template<>
template<>
void testObj::test<2>(void)
{
  const AccountConfig ac( getTestConfig() );
  gloox::JID jid(ac.getLogin() + "@" + ac.getServer() + "/acarm-ng");
  AutoSession           ags( new gloox::Client(jid, ac.getPassword()) );
  ensure("conneciton error", ags.get()!=NULL );
}

// test ownership passing
template<>
template<>
void testObj::test<3>(void)
{
  const AccountConfig ac( getTestConfig() );
  gloox::JID jid(ac.getLogin() + "@" + ac.getServer() + "/acarm-ng");
  AutoSession         ags( new gloox::Client(jid, ac.getPassword()) );
  ensure("conneciton error", ags.get()!=NULL );
  AutoSession         out;
  ensure("NULL-initialization failed", out.get()==NULL );
  out=ags;      // move ownership
  ensure("source not NULLed", ags.get()==NULL );
  ensure("out not set", out.get()!=NULL );
}

} // namespace tut
