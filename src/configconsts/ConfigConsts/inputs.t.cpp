/*
 * inputs.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "ConfigConsts/inputs.hpp"
#include "ConfigConsts/TestBase.t.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/inputs");
} // unnamed namespace


namespace tut
{

// test default input type
template<>
template<>
void testObj::test<1>(void)
{
  testString(defaultInputTypeName, "*input*");
}

// test default input instance
template<>
template<>
void testObj::test<2>(void)
{
  testString(defaultInputInstanceName, "*unknown*");
}

// test input timeout avlue
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("invalid timeout for input", inputTimeout, 21);
}

// test input queue size
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid size of the input queue", inputQueueSize, 32);
}

} // namespace tut
