/*
 * Who.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/SnortSam/Who.hpp"

using namespace Trigger::SnortSam;

namespace
{

struct TestClass
{
  void foo(const Who)
  {
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/SnortSam/Who");
} // unnamed namespace


namespace tut
{

// test NONE
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invelid NONE value", Who::NONE, 0);
}

// test SRC
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invelid SRC value", Who::SRC, 1);
}

// test DST
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invelid DST value", Who::DST, 2);
}

// test or value
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid or'ed value", (Who::SRC|Who::DST).toInt(), 3);
}

// test or value of the same types
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid or'ed value", (Who::DST|Who::DST).toInt(), Who::DST);
}

// test passing to a function
template<>
template<>
void testObj::test<6>(void)
{
  foo(Who::DST|Who::SRC);
}

} // namespace tut
