/*
 * MemoryUseChecker.t.cpp
 *
 */
#include <tut/tut.hpp>
#include "Core/Types/SignedNode.hpp"
#include "Core/MemoryUseChecker.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Core;
using namespace Core::Types;
using namespace TestHelpers::Persistency;

namespace
{
struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void)
  {
    defaultConfig();
    Persistency::GraphNodePtrNN node1=makeNewLeaf();
    SignedNode sn1(node1, Proc::TypeName("abc"), Proc::InstanceName("def") );
    queue.push(sn1);

    Persistency::GraphNodePtrNN node2=makeNewLeaf();
    SignedNode sn2(node2, Proc::TypeName("abc"), Proc::InstanceName("def") );
    queue.push(sn2);
  }

  Core::Types::SignedNodesFifo queue;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/MemoryUseChecker");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  MemoryUseChecker pl(queue);
}

// test for himem scenario
template<>
template<>
void testObj::test<2>(void)
{
  MemoryUseChecker pl(queue);
  tut::ensure_equals("Number of alerts is exceeded but should not be.", pl.alertsLimitExceeded(), false);
}

// test for lomem scenario
template<>
template<>
void testObj::test<3>(void)
{
  readConfigFile("testdata/low_mem.xml");
  MemoryUseChecker pl(queue);
  tut::ensure_equals("Number of alerts is not exceeded but should be.", pl.alertsLimitExceeded(), true);
}

} // namespace tut
