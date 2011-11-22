/*
 * configio.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "ConfigConsts/configio.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass
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
  ensure("string is NULL", configRootNodeName!=NULL);
  ensure_equals("string is valid", configRootNodeName, std::string("acarm_ng") );
}

} // namespace tut
