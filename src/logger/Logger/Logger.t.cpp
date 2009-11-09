/*
 * Logger.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cstring>

#include "Logger/Logger.hpp"

using namespace tut;
using namespace std;
using namespace Logger;


namespace
{
struct LoggerTestClass
{
  LoggerTestClass():
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
    ensure("invalid call name", strstr(call, "LoggerTestClass")!=NULL);

    ensure("line is not valid",     line>30);

    ensure("msg paramter is null",  msg!=NULL);
    ensure_equals("invalid message", string(msg), "test");
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

  Node        n_;
  mutable int calls_[5];
};

typedef LoggerTestClass TestClass;
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

} // namespace tut
