/*
 * LogCallback.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>

#include "Input/Prelude/LogCallback.hpp"

using namespace std;
using namespace Input::Prelude;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Prelude/LogCallback");
} // unnamed namespace


namespace tut
{

// test craeting single instance
template<>
template<>
void testObj::test<1>(void)
{
  LogCallback lc("test");
}

// test craeting multiple instances, one after another
template<>
template<>
void testObj::test<2>(void)
{
  // 1st
  {
    LogCallback lc("test");
  }
  // 2nd
  {
    LogCallback lc("test");
  }
}

} // namespace tut
