/*
 * Connection.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Jabber/Connection.hpp"
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

factory tf("Trigger/Jabber/Connection");
} // unnamed namespace


namespace tut
{

// test connecting
template<>
template<>
void testObj::test<1>(void)
{
  const Connection conn( getTestConfig() );
  ensure("get() returns error", conn.get()!=NULL );
}

// test throw on invalid jabber server
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    Connection conn( AccountConfig("jabber.wrong", "acarm-ng1", "testaccount") );
    fail("connectiong to invalid account didn't failed");
  }
  catch(const ExceptionConnectionError &)
  {
    // this is expected
  }
}

// test throw on invalid password
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    Connection conn( AccountConfig("jabber.org", "acarm-ng1", "testaccoun") );
    fail("connectiong to invalid account didn't failed");
  }
  catch(const ExceptionConnectionError &)
  {
    // this is expected
  }
}

// test throw on invalid login
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    Connection conn( AccountConfig("jabber.org", "acarm-wrong", "testaccount") );
    fail("connectiong to invalid account didn't failed");
  }
  catch(const ExceptionConnectionError &)
  {
    // this is expected
  }
}
} // namespace tut
