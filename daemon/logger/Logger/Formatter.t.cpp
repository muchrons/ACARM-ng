/*
 * Formatter.t.cpp
 *
 */
#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/Formatter.hpp"

namespace
{
struct FormatterTestClass: private TestHelpers::TestBase
{
  FormatterTestClass():
    fmt_()
  {
  }

  void testFormatting(const Logger::Priority pri, const std::string &priStr) const
  {
    std::stringstream ss;
    timeb             ts={4,2,0,0};
    fmt_.format(ss, ts, "node.1", pri, "file", "call", 123, "hello log!");
    tut::ensure_equals("invalid format", ss.str(),
                       priStr + "@4.002/node.1 file:123 call: hello log!");
  }

  const Logger::Formatter fmt_;
};

typedef FormatterTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/Formatter");
} // unnamed namespace


namespace tut
{

using namespace Logger;

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
    timeb             ts={4,2,0,0};
    fmt_.format(ss, ts, "node.1", Priority::DEBUG, NULL, NULL, 123, NULL);
    tut::ensure_equals("invalid format", ss.str(),
                       "DEBUG@4.002/node.1 NULL:123 NULL: NULL");
}

// smoke test for swap()
template<>
template<>
void testObj::test<7>(void)
{
  Formatter f1;
  Formatter f2;

  f1.swap(f2);
}

} // namespace tut
