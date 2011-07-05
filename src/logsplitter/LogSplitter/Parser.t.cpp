/*
 * Parser.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "LogSplitter/Parser.hpp"

using namespace LogSplitter;

namespace
{
struct TestClass
{
  void ensureThrow(const char *str) const
  {
    try
    {
      const Parser p(str);
      tut::fail("parsing didn't failed for invalid input");
    }
    catch(const std::runtime_error &)
    {
      // this is expected
    }
  }

  void ensureValid(const char *str, unsigned int num) const
  {
    const Parser p(str);
    tut::ensure_equals("invalid value parsed", p.get(), num);
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("LogSplitter/Parser");
} // unnamed namespace


namespace tut
{

// test failed parse when string is empty
template<>
template<>
void testObj::test<1>(void)
{
  ensureThrow("");
}

// test failure on some random chars
template<>
template<>
void testObj::test<2>(void)
{
  ensureThrow("sdflo23jwds0)");
}

// test failure when string is too short
template<>
template<>
void testObj::test<3>(void)
{
  ensureThrow(" ");
}

// test failure when string is too short
template<>
template<>
void testObj::test<4>(void)
{
  ensureThrow(" [");
}

// test failure when string is too short
template<>
template<>
void testObj::test<5>(void)
{
  ensureThrow(" [1");
}

// test failure when string is too short
template<>
template<>
void testObj::test<6>(void)
{
  ensureThrow(" [12");
}

// test failure when string is too short
template<>
template<>
void testObj::test<7>(void)
{
  ensureThrow(" [12]");
}

// test parse for valid string
template<>
template<>
void testObj::test<8>(void)
{
  ensureValid(" [12] ", 12);
}

// test minimal working example
template<>
template<>
void testObj::test<9>(void)
{
  ensureValid(" [1] ", 1);
}

// test parse error when brackets are empty
template<>
template<>
void testObj::test<10>(void)
{
  ensureThrow(" [] ");
}

// test error when non-digit is in brackets
template<>
template<>
void testObj::test<11>(void)
{
  ensureThrow(" [x] ");
}

// test error when non-digit is placed inside the brackets
template<>
template<>
void testObj::test<12>(void)
{
  ensureThrow(" [123x567] ");
}

// test longer number
template<>
template<>
void testObj::test<13>(void)
{
  ensureValid(" [12345] ", 12345);
}

} // namespace tut
