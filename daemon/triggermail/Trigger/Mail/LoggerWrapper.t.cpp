/*
 * LoggerWrapper.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Mail/LoggerWrapper.hpp"

using namespace Trigger::Mail;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Mail/LoggerWrapper");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  LoggerWrapper lw;
}

// test multiple instances one after another
template<>
template<>
void testObj::test<2>(void)
{
  {
    LoggerWrapper lw;
  }
  {
    LoggerWrapper lw;
  }
}

// test creating multiple instances at once
template<>
template<>
void testObj::test<3>(void)
{
  LoggerWrapper lw1;
  LoggerWrapper lw2;
}

} // namespace tut
