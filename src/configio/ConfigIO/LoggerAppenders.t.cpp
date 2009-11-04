/*
 * LoggerAppenders.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "ConfigIO/LoggerAppenders.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct LoggerAppendersTestClass
{
  LoggerAppendersTestClass(void)
  {
  }

  LoggerAppenderConfig mkAppCfg(const char *name) const
  {
    return LoggerAppenderConfig("some type", name,
                                LoggerAppenderConfig::Options() );
  }

  LoggerAppenders::Appenders apps_;
};

typedef LoggerAppendersTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/LoggerAppenders");
} // unnamed namespace


namespace tut
{

// test empty collection
template<>
template<>
void testObj::test<1>(void)
{
  const LoggerAppenders la(apps_);
  ensure("non-zero elements in colelction", la.begin()==la.end() );
}

// test adding two unique elements
template<>
template<>
void testObj::test<2>(void)
{
  apps_.push_back( mkAppCfg("some name 1") );
  apps_.push_back( mkAppCfg("some name 2") );
  const LoggerAppenders la(apps_);

  // check content
  LoggerAppenders::const_iterator it=la.begin();

  ensure("zero elements in colelction", it!=la.end() );
  ensure_equals("invalid first element", it->getName(), "some name 1");
  ++it;

  ensure("just one elements in colelction", it!=la.end() );
  ensure_equals("invalid second element", it->getName(), "some name 2");
  ++it;

  ensure("more than tow elements in collection", it==la.end() );
}

// test throw on buplicated entry
template<>
template<>
void testObj::test<3>(void)
{
  apps_.push_back( mkAppCfg("some name 1") );
  apps_.push_back( mkAppCfg("some name 2") );
  apps_.push_back( mkAppCfg("some name 1") );
  try
  {
    LoggerAppenders la(apps_);
    fail("LoggerAppenders() didn't throw on duplicated entries");
  }
  catch(const ExceptionDuplicatedAppenderName&)
  {
    // this is expected
  }
}

// test searching for appender
template<>
template<>
void testObj::test<4>(void)
{
  apps_.push_back( mkAppCfg("some name 1") );
  apps_.push_back( mkAppCfg("some name 2") );
  const LoggerAppenders       la(apps_);
  const LoggerAppenderConfig &lac=la["some name 1"];
  ensure_equals("invlid cofnig returned", lac.getName(), "some name 1");
}

// test throw on non-existing appender name
template<>
template<>
void testObj::test<5>(void)
{
  apps_.push_back( mkAppCfg("some name 1") );
  apps_.push_back( mkAppCfg("some name 2") );
  const LoggerAppenders la(apps_);
  try
  {
    la["non-existing name"];
    fail("LoggerAppenders::opeartor[]() didn't throw on unknown appender");
  }
  catch(const ExceptionInvalidAppenderName&)
  {
    // this is expected
  }
}

} // namespace tut
