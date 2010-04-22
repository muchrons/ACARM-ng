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
  TestClass()
  {
  }

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
  //Client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
}

template<>
template<>
void testObj::test<2>(void)
{
  //Client client("profile","/etc/prelude/default/client.conf",PRELUDE_CONNECTION_PERMISSION_IDMEF_READ);
  //client.start();
}


} // namespace tut
