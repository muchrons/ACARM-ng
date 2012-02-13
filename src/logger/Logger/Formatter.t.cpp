/*
 * Formatter.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Formatter.hpp"

using namespace Logger;

namespace
{
struct FormatterTestClass: private TestHelpers::TestBase
{
  FormatterTestClass():
    fmt_()
  {
  }

  void testFormatting(const Logger::Priority pri, const std::string &priStr)
  {
    std::stringstream ss;
    timeb             ts={4,2,0,0};
    fmt_.format(ss, ts, "node.1", pri, "file", "call", 123, "hello log!");
    tut::ensure_equals("invalid format", ss.str(), priStr + "@Thu Jan  1 01:00:04 1970.002/node.1 [0] file:123 call: hello log!");
  }

  std::string makeStr(const size_t len) const
  {
    std::stringstream ss;
    for(size_t i=0; i<len; ++i)
      ss<<"x";
    return ss.str();
  }

  void testValidForBufferSize(const size_t len)
  {
    std::stringstream ss;
    const std::string str=makeStr(len);
    const timeb       ts={4,2,0,0};
    fmt_.format(ss, ts, "node.1", Priority::DEBUG, NULL, NULL, 123, str.c_str() );
    tut::ensure_equals("invalid message content", ss.str(), "DEBUG@Thu Jan  1 01:00:04 1970.002/node.1 [0] NULL:123 NULL: "+str);
  }

  Logger::Formatter fmt_;
};

typedef FormatterTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Formatter");
} // unnamed namespace


namespace tut
{

// test for debug
template<>
template<>
void testObj::test<1>(void)
{
  testFormatting(Priority::DEBUG, "DEBUG");
}

// test for info
template<>
template<>
void testObj::test<2>(void)
{
  testFormatting(Priority::INFO, "INFO ");
}

// test for warning
template<>
template<>
void testObj::test<3>(void)
{
  testFormatting(Priority::WARN, "WARN ");
}

// test for error
template<>
template<>
void testObj::test<4>(void)
{
  testFormatting(Priority::ERROR, "ERROR");
}

// test for fatal
template<>
template<>
void testObj::test<5>(void)
{
  testFormatting(Priority::FATAL, "FATAL");
}

// test formatting for nulls
template<>
template<>
void testObj::test<6>(void)
{
  std::stringstream ss;
  const timeb       ts={4,2,0,0};
  fmt_.format(ss, ts, "node.1", Priority::DEBUG, NULL, NULL, 123, NULL);
  tut::ensure_equals("invalid format", ss.str(), "DEBUG@Thu Jan  1 01:00:04 1970.002/node.1 [0] NULL:123 NULL: NULL");
}

// test large message (over buffer size)
template<>
template<>
void testObj::test<7>(void)
{
  testValidForBufferSize(543);
}

// test replacing special chars with '.'
template<>
template<>
void testObj::test<8>(void)
{
  std::stringstream ss;
  const timeb       ts={4,2,0,0};
  fmt_.format(ss, ts, "node.1", Priority::DEBUG, NULL, NULL, 123, "A \n6\tc/\\?<,.>:;\"'[{]}+=_-)(*&^%$#@!~`\x07");
  tut::ensure_equals("special chars not removed", ss.str(),
                     "DEBUG@Thu Jan  1 01:00:04 1970.002/node.1 [0] NULL:123 NULL: A .6.c/\\?<,.>:;\"'[{]}+=_-)(*&^%$#@!~`.");
}

// test formatting empty (but not NULL!) message
template<>
template<>
void testObj::test<9>(void)
{
  std::stringstream ss;
  const timeb       ts={4,2,0,0};
  fmt_.format(ss, ts, "node.1", Priority::DEBUG, NULL, NULL, 123, "");
  tut::ensure_equals("special chars not removed", ss.str(), "DEBUG@Thu Jan  1 01:00:04 1970.002/node.1 [0] NULL:123 NULL: ");
}

// test message of a buffer size
template<>
template<>
void testObj::test<10>(void)
{
  testValidForBufferSize(64);
}

// test message of a buffer size-1
template<>
template<>
void testObj::test<11>(void)
{
  testValidForBufferSize(64-1);
}

// test message of a little size (less the buffer)
template<>
template<>
void testObj::test<12>(void)
{
  testValidForBufferSize(5);
}

} // namespace tut
