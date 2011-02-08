/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/UsersMonitor/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::UsersMonitor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    s_("somename", 997)
  {
  }

  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/UsersMonitor/Strategy");
} // unnamed namespace


namespace tut
{

// test if entry is not interesting when no user is set
template<>
template<>
void testObj::test<1>(void)
{
  fail("TODO");
}

// test if entry is not interesting when no user is set
template<>
template<>
void testObj::test<2>(void)
{
  fail("TODO");
}

// test canCorrelate method
template<>
template<>
void testObj::test<3>(void)
{
  fail("TODO");
}

// test if can correltate will return false for non-overlaping entries
template<>
template<>
void testObj::test<4>(void)
{
  fail("TODO");
}

// test getting name of meta alert
template<>
template<>
void testObj::test<5>(void)
{
  fail("TODO");
}

} // namespace tut
