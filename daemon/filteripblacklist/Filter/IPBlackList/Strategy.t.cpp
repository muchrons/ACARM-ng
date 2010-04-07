/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/IPBlackList/Strategy.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    s_( Strategy::Parameters(11, 22, 0.1) )
  {
  }

  Persistency::GraphNodePtrNN makeNode(void) const
  {
    return makeNewNode( makeNewLeaf("127.0.0.2"), makeNewLeaf("127.0.0.1") );
  }

  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/Strategy");
} // unnamed namespace


namespace tut
{

// test leaf
template<>
template<>
void testObj::test<1>(void)
{
  s_.process( makeNewLeaf("127.0.0.2"), changed_ );
  ensure_equals("nothing changed", changed_.size(), 0u);
}

// test node
template<>
template<>
void testObj::test<2>(void)
{
  s_.process( makeNode(), changed_ );
  ensure_equals("nothing changed", changed_.size(), 0u);
}

} // namespace tut
