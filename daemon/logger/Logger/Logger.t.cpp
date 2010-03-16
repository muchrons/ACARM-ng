/*
 * Logger.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Logger.hpp"
#include "Logger/TestHelpers.t.hpp"

using namespace tut;
using Logger::Node;
using Logger::ensureLoggedPart;
using Logger::ensureNotLoggedPart;

namespace
{
struct TestClass: private TestHelpers::TestBase
{
  TestClass():
    n_("a.b.c")
  {
    for(unsigned int i=0; i<sizeof(calls_)/sizeof(calls_[0]); ++i)
      calls_[i]=0;
  }

  void testLog(const char   *file,
               const char   *call,
               unsigned int  line,
               const char   *msg) const
  {
    ensure("file paramter is null", file!=NULL);
    ensure("invalid file name", strstr(file, "Logger.t.cpp")!=NULL);

    ensure("call paramter is null", call!=NULL);
    ensure("invalid call name", strstr(call, "TestClass")!=NULL);

    ensure("line is not valid",     line>30);

    ensure("msg paramter is null",  msg!=NULL);
    ensure_equals("invalid message", std::string(msg), "test");
  }

  void debug(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    testLog(file, call, line, msg);
    ++calls_[0];
  }

  void info( const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    testLog(file, call, line, msg);
    ++calls_[1];
  }

  void warn( const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    testLog(file, call, line, msg);
    ++calls_[2];
  }

  void error(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    testLog(file, call, line, msg);
    ++calls_[3];
  }

  void fatal(const char   *file,
             const char   *call,
             unsigned int  line,
             const char   *msg) const
  {
    testLog(file, call, line, msg);
    ++calls_[4];
  }

  void ensureCalls(unsigned int n) const
  {
    for(unsigned int i=0; i<sizeof(calls_)/sizeof(calls_[0]); ++i)
      if(i==n)
        ensure_equals("invalid calls number", calls_[i], 1);
      else
        ensure_equals("invalid call made", calls_[i], 0);
  }

  Logger::Node n_;
  mutable int  calls_[5];
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Logger");
} // unnamed namespace


namespace tut
{

// test logging debug
template<>
template<>
void testObj::test<1>(void)
{
  LOGMSG_DEBUG(*this, "test");
  ensureCalls(0);
}

// test logging info
template<>
template<>
void testObj::test<2>(void)
{
  LOGMSG_INFO(*this, "test");
  ensureCalls(1);
}

// test logging warning
template<>
template<>
void testObj::test<3>(void)
{
  LOGMSG_WARN(*this, "test");
  ensureCalls(2);
}

// test logging error
template<>
template<>
void testObj::test<4>(void)
{
  LOGMSG_ERROR(*this, "test");
  ensureCalls(3);
}

// test logging fatal
template<>
template<>
void testObj::test<5>(void)
{
  LOGMSG_FATAL(*this, "test");
  ensureCalls(4);
}

// test stream logging - empty message.
template<>
template<>
void testObj::test<6>(void)
{
  LOGMSG_DEBUG_S(n_);
}

// test stream logging debug message
template<>
template<>
void testObj::test<7>(void)
{
  LOGMSG_DEBUG_S(n_)<<"hello "<<"debug"<<" stream!";
  ensureLoggedPart("hello debug stream!");
  ensureLoggedPart("DEBUG@");
}

// test stream logging info message
template<>
template<>
void testObj::test<8>(void)
{
  LOGMSG_INFO_S(n_)<<"hello "<<"info"<<" stream!";
  ensureLoggedPart("hello info stream!");
  ensureLoggedPart("INFO @");
}

// test stream logging warning message
template<>
template<>
void testObj::test<9>(void)
{
  LOGMSG_WARN_S(n_)<<"hello "<<"warn"<<" stream!";
  ensureLoggedPart("hello warn stream!");
  ensureLoggedPart("WARN @");
}

// test stream logging error message
template<>
template<>
void testObj::test<10>(void)
{
  LOGMSG_ERROR_S(n_)<<"hello "<<"error"<<" stream!";
  ensureLoggedPart("hello error stream!");
  ensureLoggedPart("ERROR@");
}

// test stream logging fatal message
template<>
template<>
void testObj::test<11>(void)
{
  LOGMSG_FATAL_S(n_)<<"hello "<<"fatal"<<" stream!";
  ensureLoggedPart("hello fatal stream!");
  ensureLoggedPart("FATAL@");
}

// thresholding for debug
template<>
template<>
void testObj::test<12>(void)
{
  const Node  log("root");
  const char *msg="hello debug 12";
  LOGMSG_DEBUG(log, msg);
  ensureLoggedPart(msg);
  ensureLoggedPart("DEBUG@");
}

// thresholding for warn / 1
template<>
template<>
void testObj::test<13>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 13";
  LOGMSG_DEBUG(log, msg);
  ensureNotLoggedPart(msg);
}

// thresholding for warn / 2
template<>
template<>
void testObj::test<14>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 14";
  LOGMSG_INFO(log, msg);
  ensureNotLoggedPart(msg);
}

// thresholding for warn / 3
template<>
template<>
void testObj::test<15>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 15";
  LOGMSG_WARN(log, msg);
  ensureLoggedPart(msg);
}

// thresholding for warn / 4
template<>
template<>
void testObj::test<16>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 16";
  LOGMSG_ERROR(log, msg);
  ensureLoggedPart(msg);
}

// thresholding for warn / 5
template<>
template<>
void testObj::test<17>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 17";
  LOGMSG_FATAL(log, msg);
  ensureLoggedPart(msg);
}

// thresholding for warn / 1 (stream)
template<>
template<>
void testObj::test<18>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 18";
  LOGMSG_DEBUG_S(log)<<msg;
  ensureNotLoggedPart(msg);
}

// thresholding for warn / 2 (stream)
template<>
template<>
void testObj::test<19>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 19";
  LOGMSG_INFO_S(log)<<msg;
  ensureNotLoggedPart(msg);
}

// thresholding for warn / 3 (stream)
template<>
template<>
void testObj::test<20>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 20";
  LOGMSG_WARN_S(log)<<msg;
  ensureLoggedPart(msg);
}

// thresholding for warn / 4 (stream)
template<>
template<>
void testObj::test<21>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 21";
  LOGMSG_ERROR_S(log)<<msg;
  ensureLoggedPart(msg);
}

// thresholding for warn / 5 (stream)
template<>
template<>
void testObj::test<22>(void)
{
  const Node  log("subtree1.subthreshold");
  const char *msg="hello warn 22";
  LOGMSG_FATAL_S(log)<<msg;
  ensureLoggedPart(msg);
}

} // namespace tut
