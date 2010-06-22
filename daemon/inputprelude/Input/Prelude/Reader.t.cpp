/*
 * Reader.t.cpp
 *
 */
#include <tut.h>
// TODO: what are these headers for?
#include <cstring>
#include <memory>
#include <string>
#include <iostream> // TODO: direct output on the screen must not be used

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

// TODO
template<>
template<>
void testObj::test<1>(void)
{
  // TODO
}

} // namespace tut
