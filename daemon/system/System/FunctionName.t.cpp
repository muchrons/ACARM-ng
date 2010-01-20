/*
 * FunctionName.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <cstring>

#include "System/FunctionName.hpp"

namespace
{
struct TestClass
{
  void someTestFunctionName(void) const
  {
    tut::ensure("invalid name",
                strstr(SYSTEM_FUNCTION_NAME, "someTestFunctionName")!=NULL );
  }
};

typedef TestClass                  TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/FunctionName");
} // unnamed namespace


namespace tut
{

// test if required substring is present
template<>
template<>
void testObj::test<1>(void)
{
  ensure("invalid name", strstr(SYSTEM_FUNCTION_NAME, "test")!=NULL );
}

// test other (longer) name
template<>
template<>
void testObj::test<2>(void)
{
  someTestFunctionName();
}

} // namespace tut
