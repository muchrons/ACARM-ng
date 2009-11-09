/*
 * ConfigSetSingleton.t.cpp
 *
 */
#include <tut.h>

#include "Logger/detail/ConfigSetSingleton.hpp"

using namespace Logger;

namespace
{

struct ConfigSetSingletonTestClass
{
};

typedef ConfigSetSingletonTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/detail/ConfigSetSingleton");
} // unnamed namespace


namespace tut
{

} // namespace tut
