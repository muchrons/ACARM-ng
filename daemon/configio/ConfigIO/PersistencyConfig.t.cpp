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
  const PersistencyConfig pc("mytype", "user", "pass", "host", 42, "db1");
  ensure_equals("invalid type",          pc.getType(),         "mytype");
  ensure_equals("invalid user name",     pc.getUser(),         "user"  );
  ensure_equals("invalid password",      pc.getPassword(),     "pass"  );
  ensure_equals("invalid host name",     pc.getHost(),         "host"  );
  ensure_equals("invalid port number",   pc.getPortNumber(),   42      );
  ensure_equals("invalid database name", pc.getDataBaseName(), "db1"   );
}

// test throwing on invalid port number
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    PersistencyConfig pc("type", "user", "pass", "host", 0, "db1");
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
  const PersistencyConfig pc1("type", "user", "pass", "host", 42, "db1");
  PersistencyConfig       pc2=pc1;
  pc2=pc1;

  // check if all fields equals
  ensure_equals("invalid type",        pc1.getType(),         pc2.getType()         );
  ensure_equals("invalid user name",   pc1.getUser(),         pc2.getUser()         );
  ensure_equals("invalid password",    pc1.getPassword(),     pc2.getPassword()     );
  ensure_equals("invalid host name",   pc1.getHost(),         pc2.getHost()         );
  ensure_equals("invalid port number", pc1.getPortNumber(),   pc2.getPortNumber()   );
  ensure_equals("invalid db-name",     pc1.getDataBaseName(), pc2.getDataBaseName() );
}

} // namespace tut
