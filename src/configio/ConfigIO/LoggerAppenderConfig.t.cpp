/*
 * LoggerAppenderConfig.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "ConfigIO/LoggerAppenderConfig.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct LoggerAppenderConfigTestClass
{
  LoggerAppenderConfigTestClass(void)
  {
    opts_.insert( make_pair( string("name1"), string("value1") ) );
    opts_.insert( make_pair( string("xyz"),   string("narf") )   );
  }

  LoggerAppenderConfig::Options opts_;
};

typedef LoggerAppenderConfigTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/LoggerAppenderConfig");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const LoggerAppenderConfig lac("type", "name", opts_);
}

// test getting name
template<>
template<>
void testObj::test<2>(void)
{
  const LoggerAppenderConfig lac("type", "name", opts_);
  ensure_equals("invalid name", lac.getName(), "name");
}

// test getting type
template<>
template<>
void testObj::test<3>(void)
{
  const LoggerAppenderConfig lac("type", "name", opts_);
  ensure_equals("invalid type", lac.getType(), "type");
}

// test getting options that exist
template<>
template<>
void testObj::test<4>(void)
{
  const LoggerAppenderConfig lac("type", "name", opts_);
  ensure_equals("option 'name1' has invalid value", lac["name1"], "value1");
  ensure_equals("option 'xyz' has invalid value",   lac["xyz"],   "narf");
}

// test getting non-exisitng option
template<>
template<>
void testObj::test<5>(void)
{
  const LoggerAppenderConfig lac("type", "name", opts_);
  try
  {
    lac["non-existing-option"];
    fail("LoggerAppenderConfig::operator[] didn't throw on nonexisting option");
  }
  catch(const ExceptionInvalidOptionName&)
  {
    // this is expected
  }
}

} // namespace tut
