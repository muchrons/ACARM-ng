/*
 * Tree.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Compose/Tree.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger::Compose;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Compose/Tree");
} // unnamed namespace


namespace tut
{

// test creating report
template<>
template<>
void testObj::test<1>(void)
{
  std::stringstream ss;
  Tree::append( ss, makeNewNode() );
  ensure_equals("invalid report",
                ss.str(),
                "some meta-alert\n"
                "  some alert\n"
                "  some alert\n");
}

} // namespace tut
