/*
 * Parser.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <unistd.h>
#include <stdlib.h>

#include "ConfigIO/Parser.hpp"

using namespace std;
using namespace ConfigIO;

namespace
{
struct TestClass
{
};

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
  ensure_equals("invalid type", pc.getType(), "postgres");
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
  catch(const System::DiskFile::ExceptionCannotOpenFile &)
  {
    // this is not expected.
  }
}

// test default file parameter - should not throw
template<>
template<>
void testObj::test<5>(void)
{
  Parser p;
}

// test throw when default file does not exist
template<>
template<>
void testObj::test<6>(void)
{
  const Parser p("testdata/sample_config.xml");
  const LoggerConfig &lc=p.getLoggerConfig();
  // check random field
  ensure_equals("invalid default appender",
                lc.getDefaultNodeConfig().getAppenderName(), "default");
}

// test getting filters' configuration - smoke test
template<>
template<>
void testObj::test<7>(void)
{
  const Parser p("testdata/sample_config.xml");
  p.getFiltersConfig();
}

// test getting triggers' configuration - smoke test
template<>
template<>
void testObj::test<8>(void)
{
  const Parser p("testdata/sample_config.xml");
  p.getTriggersConfig();
}

// test getting inputs' configuration - smoke test
template<>
template<>
void testObj::test<9>(void)
{
  const Parser p("testdata/sample_config.xml");
  p.getInputsConfig();
}

// test getting preprocessor's configuration - smoke test
template<>
template<>
void testObj::test<10>(void)
{
  const Parser p("testdata/sample_config.xml");
  p.getPreprocessorConfig();
}

// test exception when 'general' section is missing
template<>
template<>
void testObj::test<11>(void)
{
  try
  {
    Parser p("testdata/missing_general_section.xml");
    fail("parsing didn't failed on missing 'general' section");
  }
  catch(const XML::Exception &)
  {
    // this is expected
  }
}

// test getting preprocessor's configuration in trigger
template<>
template<>
void testObj::test<12>(void)
{
  const Parser                p("testdata/trigger_preproc.xml");
  const TriggerConfig        &cfg  =p.getTriggersConfig().at(0);
  ensure_equals("invalid type name", cfg.getType(), "trig_with_pp");
  const Preprocessor::Config *ppcfg=cfg.getPreprocessorConfig();
  ensure("pointer is NULL", ppcfg!=NULL);
  ensure_equals("invalid number of rules", ppcfg->getSections().size(), 2u);
}

} // namespace tut
