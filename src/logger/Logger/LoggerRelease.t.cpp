/*
 * LoggerRelease.t.cpp
 *
 */

// remove debug logging macros
#define NDEBUG

#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Logger.hpp"
#include "Logger/TestHelpers.t.hpp"

using namespace tut;
using Logger::ensureLoggedPart;
using Logger::ensureNotLoggedPart;

namespace
{
struct TestClass: private TestHelpers::TestBase
{
  TestClass(void):
    n_("a.b.c")
  {
  }

  const Logger::Node n_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/LoggerRelease");
} // unnamed namespace


namespace tut
{

// test stream logging - empty message.
template<>
template<>
void testObj::test<1>(void)
{
  LOGMSG_DEBUG_S(n_);
}

// test stream logging debug message (removed in release mode)
template<>
template<>
void testObj::test<2>(void)
{
  LOGMSG_DEBUG_S(n_)<<"hello "<<"debug"<<" stream!";
  ensureNotLoggedPart("hello debug stream!");
  ensureNotLoggedPart("DEBUG@");
}

// test stream logging info message
template<>
template<>
void testObj::test<3>(void)
{
  LOGMSG_INFO_S(n_)<<"hello "<<"info"<<" stream!";
  ensureLoggedPart("hello info stream!");
  ensureLoggedPart("INFO @");
}

// test stream logging warning message
template<>
template<>
void testObj::test<4>(void)
{
  LOGMSG_WARN_S(n_)<<"hello "<<"warn"<<" stream!";
  ensureLoggedPart("hello warn stream!");
  ensureLoggedPart("WARN @");
}

// test stream logging error message
template<>
template<>
void testObj::test<5>(void)
{
  LOGMSG_ERROR_S(n_)<<"hello "<<"error"<<" stream!";
  ensureLoggedPart("hello error stream!");
  ensureLoggedPart("ERROR@");
}

// test stream logging fatal message
template<>
template<>
void testObj::test<6>(void)
{
  LOGMSG_FATAL_S(n_)<<"hello "<<"fatal"<<" stream!";
  ensureLoggedPart("hello fatal stream!");
  ensureLoggedPart("FATAL@");
}


namespace
{
int throwWhenCalled(void)
{
  fail("throwWhenCalled() has not been compiled-out - debug calls are left in code");
  return 42;        // note: we never reach here
} // throwWhenCalled()
} // unnamed namespace

// test compiling out debug stream's content
template<>
template<>
void testObj::test<7>(void)
{
  LOGMSG_DEBUG_S(n_)<<"test compiling-out "<<throwWhenCalled();
  ensureNotLoggedPart("test compiling-out");
}

// test if/else for simple logging macro
template<>
template<>
void testObj::test<8>(void)
{
  if(true)
    LOGMSG_DEBUG(n_, "true logged 1");
  else
    LOGMSG_DEBUG(n_, "false logged 1");

  ensureNotLoggedPart("true logged 1");
  ensureNotLoggedPart("false logged 1");
}

// test if/else for stream logging macro
template<>
template<>
void testObj::test<9>(void)
{
  if(true)
    LOGMSG_DEBUG_S(n_)<<"true logged 2";
  else
    LOGMSG_DEBUG_S(n_)<<"false logged 2";

  ensureNotLoggedPart("true logged 2");
  ensureNotLoggedPart("false logged 2");
}

} // namespace tut
