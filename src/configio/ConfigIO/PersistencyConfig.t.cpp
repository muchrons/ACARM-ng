/*
 * PersistencyConfig.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/PersistencyConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct PersistencyConfigTestClass
{
};

typedef PersistencyConfigTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/PersistencyConfig");
} // unnamed namespace


namespace tut
{

// test creating valid data
template<>
template<>
void testObj::test<1>(void)
{
  const PersistencyConfig pc("user", "pass", "host", 42);
  ensure_equals("invalid user name",   pc.getUser(),       "user");
  ensure_equals("invalid password",    pc.getPassword(),   "pass");
  ensure_equals("invalid host name",   pc.getHost(),       "host");
  ensure_equals("invalid port number", pc.getPortNumber(), 42    );
}

// test throwing on invalid port number
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    PersistencyConfig pc("user", "pass", "host", 0);
    fail("PersistencyCOnfig didn't throw on onvali port number");
  }
  catch(const ExceptionInvalidPortNumber&)
  {
    // this is expected
  }
}

// object should by copyable
template<>
template<>
void testObj::test<3>(void)
{
  const PersistencyConfig pc1("user", "pass", "host", 42);
  PersistencyConfig       pc2=pc1;
  pc2=pc1;

  // check if all fields equals
  ensure_equals("invalid user name",   pc1.getUser(),       pc2.getUser()       );
  ensure_equals("invalid password",    pc1.getPassword(),   pc2.getPassword()   );
  ensure_equals("invalid host name",   pc1.getHost(),       pc2.getHost()       );
  ensure_equals("invalid port number", pc1.getPortNumber(), pc2.getPortNumber() );
}

} // namespace tut
