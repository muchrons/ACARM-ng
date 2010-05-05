/*
 * NodeName.t.cpp
 *
 */
#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/NodeName.hpp"

using namespace Logger;

namespace
{
struct TestClass: private TestHelpers::TestBase
{
  void testRemovingChars(const char *msg, const std::string &in, const std::string &out) const
  {
    tut::ensure_equals(msg, NodeName::removeInvalidChars(in), out);
    try
    {
      NodeName nn( NodeName::removeInvalidChars(in).c_str() );
    }
    catch(...)
    {
      // just convert to more readable version
      tut::fail( ("invalid string has been generated for input: " + in).c_str() );
    }
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/NodeName");
} // unnamed namespace


namespace tut
{

// normal c-tor (smoke test - should not throw)
template<>
template<>
void testObj::test<1>(void)
{
  const NodeName nn("alice.has.a.cat");
}

// get() test
template<>
template<>
void testObj::test<2>(void)
{
  const NodeName nn("cat.bert");
  ensure_equals("get() failed", nn.get(), "cat.bert");
}

// test numbers in name
template<>
template<>
void testObj::test<3>(void)
{
  const NodeName nn("1.2.3.node");
}

// test empty name
template<>
template<>
void testObj::test<4>(void)
{
  const NodeName nn("");
}

// test throw on NULL
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    const NodeName nn(NULL);
    fail("NodeName(NULL) didn't throw on NULL");
  }
  catch(const ExceptionNullParameter &)
  {
    // this is expected
  }
}

// test making child node
template<>
template<>
void testObj::test<6>(void)
{
  const NodeName parent("abc.def");
  const NodeName child(parent, "ghi.jkl");
  ensure_equals("invalid child name", child.get(), "abc.def.ghi.jkl");
}

// test throw on invalid surfix
template<>
template<>
void testObj::test<7>(void)
{
  const NodeName parent("abc.def");
  try
  {
    const NodeName child(parent, "InvaliD");
    fail("NodeName(parent, surfix) didn't thorw on invalid surfix");
  }
  catch(const ExceptionInvalidNodeName &)
  {
    // this is expected
  }
}

// test throw on uppercase
template<>
template<>
void testObj::test<8>(void)
{
  try
  {
    const NodeName nn("invalidUpper");
    fail("NodeName() didn't throw on upper-case");
  }
  catch(const ExceptionInvalidNodeName &)
  {
    // this is expected
  }
}

// test throw on NULL surfix
template<>
template<>
void testObj::test<9>(void)
{
  const NodeName parent("abc.def");
  try
  {
    const NodeName child(parent, NULL);
    fail("NodeName(parent, NULL) didn't thorw on NULL surfix");
  }
  catch(const ExceptionNullParameter &)
  {
    // this is expected
  }
}

// test copy-c-tor of an object
template<>
template<>
void testObj::test<10>(void)
{
  const NodeName n1("abc.def");
  const NodeName n2(n1);
}

// test removing invalid chars from valid string
template<>
template<>
void testObj::test<11>(void)
{
  testRemovingChars("valid string changed", "allok", "allok");
}

// test removing invalid chars
template<>
template<>
void testObj::test<12>(void)
{
  testRemovingChars("invalid string parsing failed", "/ot&0k", "ot0k");
}

// test when all chars are invalid
template<>
template<>
void testObj::test<13>(void)
{
  testRemovingChars("invalid string w/out valid chars", "!@#", "unnamed");
}

// test replacing upper-case letters with lower-case equivalents
template<>
template<>
void testObj::test<14>(void)
{
  testRemovingChars("case-convertion failed", "ALICEhasAcat", "alicehasacat");
}

} // namespace tut
