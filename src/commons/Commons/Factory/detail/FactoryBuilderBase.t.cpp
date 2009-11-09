/*
 * FactoryBuilderBase.t.cpp
 *
 */
#include <tut.h>

//#include "Commons/Factory/detail/FactoryBuilderBase.hpp"


namespace
{
struct FactoryBuilderBaseTestClass
{
};

typedef FactoryBuilderBaseTestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Factory/detail/FactoryBuilderBase");
} // unnamed namespace


namespace tut
{

template<>
template<>
void testObj::test<1>(void)
{
}

} // namespace tut
