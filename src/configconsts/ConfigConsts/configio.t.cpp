/*
 * configio.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "ConfigConsts/configio.hpp"
#include "ConfigConsts/TestBase.t.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/configio");
} // unnamed namespace


namespace tut
{

// test configio string
template<>
template<>
void testObj::test<1>(void)
{
  testString(configRootNodeName, "acarm_ng");
}

} // namespace tut
