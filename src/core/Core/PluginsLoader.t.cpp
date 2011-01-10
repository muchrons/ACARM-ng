/*
 * PluginsLoader.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "Core/PluginsLoader.hpp"
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

factory tf("Core/PluginsLoader");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  PluginsLoader pl;
}

// test if loading invalid plugins directory fails
template<>
template<>
void testObj::test<2>(void)
{
  readConfigFile("testdata/invalid_plugins_dir.xml");
  try
  {
    PluginsLoader pl;   // should throw
    fail("c-tor didn't throw on invalid directory");
  }
  catch(const Plugins::ExceptionInvalidDirectory &)
  {
    // this is expected
  }
}

} // namespace tut
