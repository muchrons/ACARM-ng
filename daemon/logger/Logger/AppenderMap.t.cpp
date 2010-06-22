/*
 * AppenderMap.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <unistd.h>

#include "Logger/AppenderMap.hpp"
#include "Logger/Appenders/FileUnlinker.t.hpp"
#include "TestHelpers/TestBase.hpp"
#include "System/DiskFile.hpp"

using namespace std;
using namespace Logger;

namespace
{
struct TestClass: private TestHelpers::TestBase
{
  string getFileContent(const string &path) const
  {
    System::DiskFile df(path);
    char             buf[1024+1];
    const int        size=sizeof(buf)-1;
    const int        got=read( df.get(), buf, size);
    if(got<0)
      tut::fail("reading form input file failed");
    assert(0<=got);
    assert(got<=size);
    buf[got]=0;
    return buf;
  }

  ConfigIO::LoggerAppenders::Appenders    list_;
  ConfigIO::LoggerAppenderConfig::Options opts_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/AppenderMap");
} // unnamed namespace


namespace tut
{

// test creating no appenders
template<>
template<>
void testObj::test<1>(void)
{
  ConfigIO::LoggerAppenders apps(list_);
  const AppenderMap         am(apps);
}

// test creating building non-existing appender
template<>
template<>
void testObj::test<2>(void)
{
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  try
  {
    am["i do not exist"];
    fail("getting unexisting appedner didn't throw");
  }
  catch(const AppenderMap::ExceptionNoSuchAppender&)
  {
    // this is expected
  }
}

// test throw when trying to create unkown type
template<>
template<>
void testObj::test<3>(void)
{
  list_.push_back( ConfigIO::LoggerAppenderConfig("nosuchtype", "abc", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  try
  {
    AppenderMap am(apps);
    fail("creating unexisintg appedenr type didn't failed");
  }
  catch(const AppenderMap::ExceptionNoSuchAppenderType&)
  {
    // this is expected
  }
}

// try creting console appender
template<>
template<>
void testObj::test<4>(void)
{
  list_.push_back( ConfigIO::LoggerAppenderConfig("console", "c-out", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  ensure_equals("invalid type", am["c-out"]->getTypeName(), string("console") );
}

// test creating file appender
template<>
template<>
void testObj::test<5>(void)
{
  unlink("abc.txt");
  FileUnlinker fu("abc.txt");
  opts_["output"].push_back("abc.txt");
  list_.push_back( ConfigIO::LoggerAppenderConfig("file", "log", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  ensure_equals("invalid type", am["log"]->getTypeName(), string("file") );
}

// test creating multi appender (empty)
template<>
template<>
void testObj::test<6>(void)
{
  list_.push_back( ConfigIO::LoggerAppenderConfig("multiappender", "ma", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  try
  {
    AppenderMap am(apps);
    fail("creating empty multi-appender didn't failed");
  }
  catch(const ConfigIO::ExceptionInvalidOptionName &)
  {
    // this is expected
  }
}

// test creating multi-appender
template<>
template<>
void testObj::test<7>(void)
{
  // add two console appedners first
  list_.push_back( ConfigIO::LoggerAppenderConfig("console", "cons1", opts_) );
  list_.push_back( ConfigIO::LoggerAppenderConfig("console", "cons2", opts_) );

  // mark them as to be used by multi-appender
  opts_["forward"].push_back("cons2");
  opts_["forward"].push_back("cons1");
  list_.push_back( ConfigIO::LoggerAppenderConfig("multiappender", "ma", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  ensure_equals("invalid type", am["ma"]->getTypeName(), string("multiappender") );
}

// test creating null appender
template<>
template<>
void testObj::test<8>(void)
{
  list_.push_back( ConfigIO::LoggerAppenderConfig("null", "ignore", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  ensure_equals("invalid type", am["ignore"]->getTypeName(), string("null") );
}

// test if configuration of file appender is valid
template<>
template<>
void testObj::test<9>(void)
{
  const char *path="file_app_cfg.txt";
  unlink(path);
  FileUnlinker fu(path);
  opts_["output"].push_back(path);
  list_.push_back( ConfigIO::LoggerAppenderConfig("file", "log", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  am["log"]->append("hello");
  // test content
  ensure_equals("invalid content", getFileContent(path), "hello\n");
}

// test if multi appender is configured properly
template<>
template<>
void testObj::test<10>(void)
{
  // configure file 1 appender
  FileUnlinker fu("file1.txt");
  {
    unlink("file1.txt");
    ConfigIO::LoggerAppenderConfig::Options tmp;
    tmp["output"].push_back("file1.txt");
    list_.push_back( ConfigIO::LoggerAppenderConfig("file", "f1", tmp) );
  }
  // configure file 2 appender
  FileUnlinker fu2("file2.txt");
  {
    unlink("file2.txt");
    ConfigIO::LoggerAppenderConfig::Options tmp;
    tmp["output"].push_back("file2.txt");
    list_.push_back( ConfigIO::LoggerAppenderConfig("file", "f2", tmp) );
  }

  // mark them as to be used by multi-appender
  opts_["forward"].push_back("f1");
  opts_["forward"].push_back("f2");
  list_.push_back( ConfigIO::LoggerAppenderConfig("multiappender", "ma", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  am["ma"]->append("narf");

  // test content
  ensure_equals("invalid contenet of file 1", getFileContent("file1.txt"), "narf\n");
  ensure_equals("invalid contenet of file 2", getFileContent("file2.txt"), "narf\n");
}

// test const-correctness (this test must compile)
template<>
template<>
void testObj::test<11>(void)
{
  list_.push_back( ConfigIO::LoggerAppenderConfig("null", "sth", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  const AppenderMap         am(apps);
  am["sth"]->getTypeName();
}

// try creating syslog appender
template<>
template<>
void testObj::test<12>(void)
{
  list_.push_back( ConfigIO::LoggerAppenderConfig("syslog", "sysl", opts_) );
  ConfigIO::LoggerAppenders apps(list_);
  AppenderMap               am(apps);
  ensure_equals("invalid type", am["sysl"]->getTypeName(), string("syslog") );
}

} // namespace tut
