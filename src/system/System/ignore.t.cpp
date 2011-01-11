/*
 * ignore.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/ignore.hpp"

using namespace System;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/ignore");
}

namespace tut
{

// test if variable is ignored indeed (otherwise warning will be produced on most compilers
template<>
template<>
void testObj::test<1>(void)
{
  const int x=42;
  ignore(x);
}

} // namespace tut
