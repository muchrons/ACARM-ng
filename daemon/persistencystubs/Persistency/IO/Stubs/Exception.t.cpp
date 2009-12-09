/*
 * Exception.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/Stubs/Exception.hpp"

using namespace Persistency::Stubs;

namespace
{
struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/Stubs/Exception");
} // unnamed namespace


namespace tut
{

// test c-tor
template<>
template<>
void testObj::test<1>(void)
{
  Exception e(CALLNAME, "ugh!");
}

} // namespace tut
