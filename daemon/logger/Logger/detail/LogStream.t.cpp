/*
 * LogStream.t.cpp
 *
 */
#include <tut.h>
#include <cstring>

#include "TestHelpers/TestBase.hpp"
#include "Logger/detail/LogStream.hpp"
#include "Logger/TestHelpers.t.hpp"

using namespace std;
using namespace Logger;
using namespace Logger::detail;

namespace
{

#define MAKE_LS(name) LogStream< &Node::debug > name(log_, "filename", "function()", 42)

struct TestClass: private TestHelpers::TestBase
{
  TestClass(void):
    log_("logger.test")
  {
  }

  Node log_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/LogStream");
} // unnamed namespace


namespace tut
{

// test no log when nothing has been added to message
template<>
template<>
void testObj::test<1>(void)
{
  const string pre=getLastLogMessage();
  {
    MAKE_LS(ls);
  }
  const string post=getLastLogMessage();
  ensure_equals("something has been logged", post, pre);
}

// try logging different data types
template<>
template<>
void testObj::test<2>(void)
{
  {
    MAKE_LS(ls);
    ls<<"string";
    ls<<42;
    ls<<'+';
    ls<<4.2;
  }
  ensureLoggedPart("string42+4.2");
}

// try multiple additions in single row
template<>
template<>
void testObj::test<3>(void)
{
  {
    MAKE_LS(ls);
    ls<<1<<2<<3;
  }
  ensureLoggedPart("123");
}

// try logging to temporary object
template<>
template<>
void testObj::test<4>(void)
{
  // 1
  LogStream< &Node::debug >(log_, "file", "f()", 42)<<'x'<<1<<2<<3;
  ensureLoggedPart("x123");

  // 2
  LogStream< &Node::debug >(log_, "file", "f()", 42)<<'y'<<4<<5<<6;
  ensureLoggedPart("y456");
}

} // namespace tut
