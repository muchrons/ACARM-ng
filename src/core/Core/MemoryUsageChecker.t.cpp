/*
 * MemoryUsageChecker.t.cpp
 *
 */
#include <tut/tut.hpp>
#include "Core/MemoryUsageChecker.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
  ~TestClass(void)
  {
    defaultConfig();
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/MemoryUsageChecker");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MemoryUsageChecker pl;
}

// test for himem scenario
template<>
template<>
void testObj::test<2>(void)
{
  MemoryUsageChecker pl;
  tut::ensure_equals("Hi mem indicated, should be low mem.", pl.iSmemoryLimitExceeded(), false);
}

// test for lomem scenario
template<>
template<>
void testObj::test<3>(void)
{
  readConfigFile("testdata/low_mem.xml");
  MemoryUsageChecker pl;
  tut::ensure_equals("Low mem indicated, should be hi mem.", pl.iSmemoryLimitExceeded(), true);
}

} // namespace tut
