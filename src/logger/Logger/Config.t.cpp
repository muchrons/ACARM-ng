/*
 * Config.t.cpp
 *
 */
#include <tut.h>
#include <boost/filesystem.hpp>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Config.hpp"
#include "Logger/Logger.hpp"

using namespace Logger;
namespace fs=boost::filesystem;

namespace
{

struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Config");
} // unnamed namespace


namespace tut
{

// somke test for reinitializing appenders
template<>
template<>
void testObj::test<1>(void)
{
  const fs::path file("loggerfile.log");
  const fs::path fileBck("loggerfile.log.bck");
  Node           log("config.test.output");
  LOGMSG_INFO(log, "create log file if does not exist");
  fs::remove(fileBck);
  fs::rename(file, fileBck);
  ensure("log file still exists", fs::exists(file)==false);
  Config::reinitAppenders();
  LOGMSG_INFO(log, "test message2");
  ensure("file not created", fs::exists(file) );
}

} // namespace tut
