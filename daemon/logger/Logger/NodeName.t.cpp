/*
 * NodeName.t.cpp
 *
 */
#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/NodeName.hpp"

namespace
{
struct NodeNameTestClass: private TestHelpers::TestBase
{
};

typedef NodeNameTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/NodeName");
} // unnamed namespace


namespace tut
{

using namespace Logger;

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

} // namespace tut
