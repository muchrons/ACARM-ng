/*
 * Strategy.t.cpp
 *
 */

#include <tut.h>

#include "Filter/NewEvent/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Filter::NewEvent;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    params_(2, 0.3)
  {
  }

  Persistency::GraphNodePtrNN makeLeaf(const char *name) const
  {
    return makeNewLeaf( makeNewAlert(name) );
  }

  Persistency::GraphNodePtrNN makeNode(void) const
  {
    return makeNewNode( makeLeaf("some alert"), makeLeaf("some other alert") );
  }

  Strategy::Parameters   params_;
  Strategy::ChangedNodes changed_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/Strategy");
} // unnamed namespace



namespace tut
{

// test leaf
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s("somename", params_);
  s.process( makeLeaf("some name"), changed_ );
  ensure_equals("something changed", changed_.size(), 1u);
  changed_.clear();
  s.process( makeLeaf("some name"), changed_ );
  ensure_equals("something changed", changed_.size(), 0u);
  changed_.clear();
  s.process( makeLeaf("some other name"), changed_ );
  ensure_equals("something changed", changed_.size(), 1u);
}

// test node
template<>
template<>
void testObj::test<2>(void)
{
  Strategy s("somename", params_);
  s.process( makeNode(), changed_ );
  ensure_equals("something changed", changed_.size(), 2u);
}

// TODO: test adding the same name after timeout has been reached for it.

// TODO: test adding the same name multiple times, in some time span and checking if it
//       has NOT been marked as unused after first entry has timeouted, but next ones are still present.

} // namespace tut
