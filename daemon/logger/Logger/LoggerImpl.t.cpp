/*
 * LoggerImpl.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Node.hpp"
#include "Logger/LoggerImpl.hpp"
#include "Logger/TestHelpers.t.hpp"

using namespace tut;


namespace
{
struct TestClass: private TestHelpers::TestBase
{
  TestClass():
    n_("a.b.c")
  {
  }

  // this is helper method - it is a little bit tricky, since it uses internal
  // macro's representation to log message here. thanks to this macro can be
  // easily tested, without need to reconfigure logger.
  void operator()(const char   *file,
                  const char   *call,
                  unsigned int  line,
                  const char   *msg) const
  {
    ensure("file paramter is null", file!=NULL);
    ensure("invalid file name", strstr(file, "LoggerImpl.t.cpp")!=NULL);

    ensure("call paramter is null", call!=NULL);
    ensure("invalid call name", strstr(call, "TestClass")!=NULL);

    ensure("line is not valid",     line>30);

    ensure("msg paramter is null",  msg!=NULL);
    ensure_equals("invalid message", std::string(msg), "test");
  }

  Logger::Node n_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/LoggerImpl");
} // unnamed namespace


namespace tut
{

// test normal call
template<>
template<>
void testObj::test<1>(void)
{
  LOGMSG_PRI_INTERNAL_IMPLEMENTATION(*this, "test");
}

// test logging when if-else is present. this is smoke test. it must compile.
template<>
template<>
void testObj::test<2>(void)
{
  if(false)
    LOGMSG_PRI_INTERNAL_IMPLEMENTATION(*this, "test");
  else
    LOGMSG_PRI_INTERNAL_IMPLEMENTATION(*this, "test");
}

// test normal, stream call
template<>
template<>
void testObj::test<3>(void)
{
  LOGMSG_PRI_INTERNAL_STREAM_IMPLEMENTATION(n_, debug)<<"impl-test";
  Logger::ensureLoggedPart("impl-test");
  Logger::ensureLoggedPart("LoggerImpl.t.cpp");
  Logger::ensureLoggedPart("test");
}

} // namespace tut
