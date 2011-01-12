/*
 * Connection.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/Connection.hpp"
#include "Trigger/GG/TestAccount.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/Connection");
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

// test throw on invalid config
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    Connection conn( AccountConfig(123, "invalidpassword") );
    fail("connectiong to invalid account didn't failed");
  }
  catch(const ExceptionConnectionError &)
  {
    // this is expected
  }
}

} // namespace tut
