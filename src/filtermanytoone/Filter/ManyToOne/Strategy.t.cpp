/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>

#include "System/ignore.hpp"
#include "Filter/ManyToOne/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Persistency;
using namespace Filter::ManyToOne;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/ManyToOne/Strategy");
} // unnamed namespace


namespace tut
{

// smoke test for the c-tor
template<>
template<>
void testObj::test<1>(void)
{
  const Strategy s(Core::Types::Proc::InstanceName("somename"), 13);
  System::ignore(s);
}

} // namespace tut
