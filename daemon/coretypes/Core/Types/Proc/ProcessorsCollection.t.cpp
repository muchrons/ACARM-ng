/*
 * ProcessorsCollection.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/Proc/ProcessorsCollection.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core::Types::Proc;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/ProcessorsCollection");
} // unnamed namespace


namespace tut
{

// smoke test creating collection's instance
template<>
template<>
void testObj::test<1>(void)
{
  ProcessorsCollection pc;
  ensure_equals("invalid size", pc.size(), 0);
}

} // namespace tut
