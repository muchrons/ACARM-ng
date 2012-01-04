/*
 * preprocessor.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <sstream>

#include "ConfigConsts/preprocessor.hpp"
#include "ConfigConsts/TestBase.t.hpp"

using namespace ConfigConsts;

namespace
{
struct TestClass: public TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("ConfigConsts/preprocessor");
} // unnamed namespace


namespace tut
{

// test presence of max context length
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid max content length", maxParseErrorContextLength, 12);
}

} // namespace tut
