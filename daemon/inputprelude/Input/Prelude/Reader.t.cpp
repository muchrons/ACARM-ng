/*
 * Reader.t.cpp
 *
 */
#include <tut.h>

#include "Input/Prelude/Reader.hpp"

using namespace std;
using namespace Input::Prelude;

namespace
{

struct TestClass
{
  TestClass():
    r_("profile","/etc/prelude/default/client.conf")
  {
  }

  Reader r_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/Reader");
} // unnamed namespace


namespace tut
{

// test if name is valid
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", r_.getName(), "prelude");
}

} // namespace tut
