/*
 * Syslog.t.cpp
 *
 */
#include <tut.h>
#include <string>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Appenders/Syslog.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::Appenders;

namespace
{

struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Appenders/Syslog");
} // unnamed namespace


namespace tut
{

// test getting name in static way
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name returned",
               Syslog::getThisTypeName(), string("syslog") );
}

// test getting name from dynamic interface
template<>
template<>
void testObj::test<2>(void)
{
  Syslog                 sapp;
  const Appenders::Base &b=sapp;
  ensure_equals("strings are not identical",
                b.getTypeName(), string( Syslog::getThisTypeName() ) );
}

// check if addresses are physicaly indentical
template<>
template<>
void testObj::test<3>(void)
{
  Syslog                 sapp;
  const Appenders::Base &b=sapp;
  ensure("pointers are not identical",
         b.getTypeName()==Syslog::getThisTypeName() );
}

// reinit() smoke test
template<>
template<>
void testObj::test<4>(void)
{
  Syslog sapp;
  sapp.reinit();
}

} // namespace tut
