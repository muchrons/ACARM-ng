/*
 * Client.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <string>

#include "Input/Prelude/Client.hpp"

using namespace std;
using namespace Input::Prelude;


namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/Client");
} // unnamed namespace


namespace tut
{

// Check if client can connect
template<>
template<>
void testObj::test<1>(void)
{
  Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
}

// Create and destroy several clients
template<>
template<>
void testObj::test<2>(void)
{
  Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
}

// Create two clients at a time
template<>
template<>
void testObj::test<3>(void)
{
  Client a("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  Client b("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
}


} // namespace tut
