/*
 * AutoGGSession.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/GG/AutoGGSession.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::GG;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/GG/AutoGGSession");
} // unnamed namespace


namespace tut
{

// test default c-tor
template<>
template<>
void testObj::test<1>(void)
{
  AutoGGSession ags;
  ensure("NULL not created by default", ags.get()==NULL );
}

} // namespace tut
