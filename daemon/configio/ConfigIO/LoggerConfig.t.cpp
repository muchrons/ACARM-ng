/*
 * LoggerConfig.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "ConfigIO/LoggerConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct LoggerConfigTestClass
{
  LoggerConfigTestClass():
    defaultNode_("", "app1"),
    nodes_( mkNodes() ),
    appenders_( mkAppenders() )
  {
  }

  LoggerNodes::Nodes mkNodes(void) const
  {
    LoggerNodes::Nodes nodes;
    nodes.push_back( LoggerNodeConfig("some.node",       "app2") );
    nodes.push_back( LoggerNodeConfig("some.other.node", "app2") );
    nodes.push_back( LoggerNodeConfig("some",            "app1") );
    return nodes;
  }

  LoggerAppenders::Appenders mkAppenders(void) const
  {
    const LoggerAppenderConfig::Options opts;
    LoggerAppenders::Appenders          appenders;
    appenders.push_back( LoggerAppenderConfig("t1", "app2", opts) );
    appenders.push_back( LoggerAppenderConfig("t2", "app1", opts) );
    appenders.push_back( LoggerAppenderConfig("t1", "app3", opts) );
    return appenders;
  }

  LoggerNodeConfig defaultNode_;
  LoggerNodes      nodes_;
  LoggerAppenders  appenders_;
};

typedef LoggerConfigTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/LoggerConfig");
} // unnamed namespace


namespace tut
{

// check default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const LoggerConfig lc(defaultNode_, nodes_, appenders_);
}

// test throw on invalid default node name
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    LoggerConfig lc( LoggerNodeConfig("some", "app1"), nodes_, appenders_);
    fail("LoggerConfig() didn't throw on invalid default node name");
  }
  catch(const ExceptionBadOrMissingDefaultNodeSetting&)
  {
    // this is expected
  }
}

// test throw on no default apender name
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    LoggerConfig lc( LoggerNodeConfig(""), nodes_, appenders_);
    fail("LoggerConfig() didn't throw on no default appender");
  }
  catch(const ExceptionBadOrMissingDefaultNodeSetting&)
  {
    // this is expected
  }
}

// test throw on invalid default appender
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    LoggerConfig lc( LoggerNodeConfig("", "oops"), nodes_, appenders_);
    fail("LoggerConfig() didn't throw on invalid default appender name");
  }
  catch(const ExceptionInvalidAppenderName&)
  {
    // this is expected
  }
}

// test throw on invalid appender for some node
template<>
template<>
void testObj::test<5>(void)
{
  LoggerNodes::Nodes tmp;
  tmp.push_back( LoggerNodeConfig("some.node.1", "app1") );
  tmp.push_back( LoggerNodeConfig("some.node.3", "oops") );
  tmp.push_back( LoggerNodeConfig("some.node.2", "app2") );
  const LoggerNodes nodes(tmp);
  try
  {
    LoggerConfig lc(defaultNode_, nodes, appenders_);
    fail("LoggerConfig() didn't throw on invalid appender name in one node");
  }
  catch(const ExceptionInvalidAppenderName&)
  {
    // this is expected
  }
}

} // namespace tut
