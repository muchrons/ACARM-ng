/*
 * Parser.t.cpp
 *
 */
#include <tut.h>

#include "ConfigIO/Parser.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct ParserTestClass
{
};

typedef ParserTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("ConfigIO/Parser");
} // unnamed namespace


namespace tut
{

// test reading valid configuration - persistency
template<>
template<>
void testObj::test<1>(void)
{
  const Parser             p("testdata/sample_config.xml");
  const PersistencyConfig &pc=p.getPersistencyConfig();
  // check some random fields
  ensure_equals("invalid type", pc.getType(),       "PostgreSQL");
  ensure_equals("invalid port", pc.getPortNumber(), 5432        );
}

// test throwing on invalid logger config (double-check)
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    Parser p("testdata/invalid_defaul_appender.xml");
    fail("parsing didn't failed on invalid default appender (logger config)");
  }
  catch(const ExceptionInvalidAppenderName&)
  {
    // this is expected
  }
}

// test reading valid configuration - logger
template<>
template<>
void testObj::test<3>(void)
{
  const Parser        p("testdata/sample_config.xml");
  const LoggerConfig &lc=p.getLoggerConfig();
  // check some random fields
  ensure_equals("invalid default appender",
                lc.getDefaultNodeConfig().getAppenderName(), "default");
  const string &tmp=lc.getAppenders()["default"].getType();
  ensure_equals("invalid default appender's type", tmp, "file");
}

// test throw when file does not exist
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    Parser p("some_nonexisting_file.xml");
    fail("Parser() didn't throw on nonexisting file");
  }
  catch(const System::Exception&)
  {
    // this is not expected.
  }
}

} // namespace tut
