/*
 * TestBase.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <stdlib.h>

#include "TestHelpers/TestBase.hpp"

using namespace TestHelpers;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/TestBase");
} // unnamed namespace


namespace tut
{

// create stubs
template<>
template<>
void testObj::test<1>(void)
{
  TestBase tb;
  ensure_equals("invalid content", system("grep -q '>postgres<' acarm_ng_config.xml"), 0);
}

// create non-stubs
template<>
template<>
void testObj::test<2>(void)
{
  TestBase tb(true);
  ensure_equals("invalid content", system("grep -q '>stubs<' acarm_ng_config.xml"), 0);
}

} // namespace tut
