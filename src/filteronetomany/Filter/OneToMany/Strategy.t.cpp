/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "System/ignore.hpp"
#include "Filter/OneToMany/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Filter::OneToMany;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/OneToMany/Strategy");
} // unnamed namespace


namespace tut
{

// smoke test for the c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const Strategy s(Core::Types::Proc::InstanceName("somename"), 42);
  System::ignore(s);
}

} // namespace tut
