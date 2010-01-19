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

// check for persistency section
template<>
template<>
void testObj::test<1>(void)
{
  TestBase tb;
  ensure_equals("no persistency section",
                system("grep -q '<persistency>' acarm_ng_config.xml"), 0);
}

// check for logger section
template<>
template<>
void testObj::test<2>(void)
{
  TestBase tb;
  ensure_equals("no logger section",
                system("grep -q '<logger>' acarm_ng_config.xml"), 0);
}

} // namespace tut
