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
  // TODO: do not use explicit c-tor/d-tor when not needed
  TestClass()
  {
  }
  // TODO: do not use explicit c-tor/d-tor when not needed
  ~TestClass()
  {
  }
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
  // TODO
  Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  //Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
}

template<>
template<>
void testObj::test<2>(void)
{
  // TODO
  //Client client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  //client.start();
}

// TODO: add test for double init prelude one after another (situation that now causes abort()).

// TODO: add test for double init prelude at one time (i.e. two instances running).

} // namespace tut
