/*
 * ConfigSetSingleton.t.cpp
 *
 */
#include <tut.h>

#include "TestHelpers/TestBase.hpp"
#include "Logger/detail/ConfigSetSingleton.hpp"

using namespace Logger;

namespace
{

struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/detail/ConfigSetSingleton");
} // unnamed namespace


namespace tut
{

// try to initialize
template<>
template<>
void testObj::test<1>(void)
{
  detail::ConfigSetSingleton::get();
}

} // namespace tut
