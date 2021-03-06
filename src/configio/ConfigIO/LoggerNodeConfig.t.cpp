/*
 * LoggerNodeConfig.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "ConfigIO/LoggerNodeConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct LoggerNodeConfigTestClass
{
};

typedef LoggerNodeConfigTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/LoggerNodeConfig");
} // unnamed namespace


namespace tut
{

// test creating node config with appender name
template<>
template<>
void testObj::test<1>(void)
{
  const LoggerNodeConfig lnc("node", "app", "warn");
  ensure("appender not set", lnc.hasAppender() );
  ensure_equals("invlaid appender name", lnc.getAppenderName(), "app");
  ensure_equals("invlaid node name",     lnc.getNodeName(),     "node");
}

// test creating node config without appender
template<>
template<>
void testObj::test<2>(void)
{
  const LoggerNodeConfig lnc("node", OptionalString(), OptionalString() );
  ensure("appender is set", !lnc.hasAppender() );
  ensure_equals("invlaid node name", lnc.getNodeName(), "node");
}

// test throw on getting appender, when it has not been set
template<>
template<>
void testObj::test<3>(void)
{
  const LoggerNodeConfig lnc("node");
  try
  {
    lnc.getAppenderName();
    fail("getAppenderName() didn't throw when appender was not set");
  }
  catch(const LoggerNodeConfig::ExceptionNoValueAssigned&)
  {
    // this is expected
  }
}

// test creating element without appender, and threshold
template<>
template<>
void testObj::test<4>(void)
{
  const LoggerNodeConfig lnc("node");
  ensure("appender is set",  lnc.hasAppender() ==false);
  ensure("threshold is set", lnc.hasThreshold()==false);
}

} // namespace tut
