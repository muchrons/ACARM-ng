/*
 * GlobalLibPreludeInit.t.cpp
 *
 */
#include <tut.h>

#include "Input/Prelude/GlobalLibPreludeInit.hpp"

using namespace std;
using namespace Input::Prelude;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/GlobalLibPreludeInit");
} // unnamed namespace


namespace tut
{

// smoke test for c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  GlobalLibPreludeInit glpi;
}

// smoke test for c-tor/d-tor called multiple times
template<>
template<>
void testObj::test<2>(void)
{
  {
    GlobalLibPreludeInit glpi;
  }
  {
    GlobalLibPreludeInit glpi;
  }
}

// smoke test for reference counting
template<>
template<>
void testObj::test<3>(void)
{
  GlobalLibPreludeInit glpi1;
  GlobalLibPreludeInit glpi2;
}

} // namespace tut
