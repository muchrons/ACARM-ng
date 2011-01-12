/*
 * ConnectionUserStubBase.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"

using namespace TestHelpers::Persistency;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("TestHelpers/Persistency/ConnectionUserStubBase");
} // unnamed namespace


namespace tut
{

// create instance (check is stubs does create properly).
template<>
template<>
void testObj::test<1>(void)
{
  ConnectionUserStubBase cusb;
}

} // namespace tut
