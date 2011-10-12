/*
 * How.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/How.hpp"

using namespace Trigger::SnortSam;

namespace
{

struct TestClass
{
  void foo(const How) const
  {
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/How");
} // unnamed namespace


namespace tut
{

// test NONE
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invelid NONE value", How::NONE, 0);
}

// test IN
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invelid IN value", How::IN, 1);
}

// test OUT
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invelid OUT value", How::OUT, 2);
}

// test or value
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid or'ed value", (How::IN|How::OUT).toInt(), 3);
}

// test or value for the same elements
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid or'ed value", (How::IN|How::IN).toInt(), How::IN);
}

// test passing to a function
template<>
template<>
void testObj::test<6>(void)
{
  foo(How::IN|How::OUT);
}

} // namespace tut
