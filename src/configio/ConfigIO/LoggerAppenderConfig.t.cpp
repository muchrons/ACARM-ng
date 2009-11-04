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
    opts_["name1"].push_back("value1");
    opts_["xyz"].push_back("narf");
    opts_["abc"].push_back("alice");
    opts_["abc"].push_back("cat");
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
  ensure_equals("option 'name1' has invalid value", lac["name1"].at(0), "value1");
  ensure_equals("option 'xyz' has invalid value",   lac["xyz"].at(0),   "narf"  );
  ensure_equals("option 'abc' has invalid value 1", lac["abc"].at(0),   "alice" );
  ensure_equals("option 'abc' has invalid value 2", lac["abc"].at(1),   "cat"   );
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
