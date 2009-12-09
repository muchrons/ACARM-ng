/*
 * Console.t.cpp
 *
 */
#include <tut.h>

#include "Logger/Appenders/Console.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

struct ConsoleTestClass
{
};

typedef ConsoleTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Console");
} // unnamed namespace


namespace tut
{

// test getting name in static way
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name returned",
               Console::getThisTypeName(), string("Console") );
}

// test getting name from dynamic interface
template<>
template<>
void testObj::test<2>(void)
{
  Console                capp;
  const Appenders::Base &b=capp;
  ensure_equals("strings are not identical",
                b.getTypeName(), string( Console::getThisTypeName() ) );
}

// check if addresses are physicaly indentical
template<>
template<>
void testObj::test<3>(void)
{
  Console                capp;
  const Appenders::Base &b=capp;
  ensure("pointers are not identical",
         b.getTypeName()==Console::getThisTypeName() );
}

} // namespace tut
