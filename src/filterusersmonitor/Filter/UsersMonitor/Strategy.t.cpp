/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/UsersMonitor/Strategy.hpp"
#include "Filter/UsersMonitor/TestBase.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::UsersMonitor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestBase
{
  TestClass(void):
    s_("somename", 997)
  {
  }

  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/UsersMonitor/Strategy");
} // unnamed namespace


namespace tut
{

// test if entry is not interesting when no user is set
template<>
template<>
void testObj::test<1>(void)
{
  for(int i=0; i<2; ++i)
  {
    GraphNodePtrNN leaf=makeNewLeaf( mkAlert() );
    s_.process(leaf, changed_);
    ensure_equals("something has been changed", changed_.size(), 0u);
  }
}

// test correlation
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN leaf1=makeNewLeaf( mkAlert("alice", "cat") );
  s_.process(leaf1, changed_);
  ensure_equals("something has been changed", changed_.size(), 0u);

  GraphNodePtrNN leaf2=makeNewLeaf( mkAlert("cat", "kszy") );
  s_.process(leaf2, changed_);
  ensure_equals("correlation failed", changed_.size(), 1u);
}

// test if can correltate will return false for non-overlaping entries
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN leaf1=makeNewLeaf( mkAlert("alice", "cat") );
  s_.process(leaf1, changed_);
  ensure_equals("something has been changed", changed_.size(), 0u);

  GraphNodePtrNN leaf2=makeNewLeaf( mkAlert("other-cat", "kszy") );
  s_.process(leaf2, changed_);
  ensure_equals("correlation didn't failed", changed_.size(), 0u);
}

// test getting name of meta alert
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN leaf1=makeNewLeaf( mkAlert("alice", "cat") );
  s_.process(leaf1, changed_);
  ensure_equals("something has been changed", changed_.size(), 0u);

  GraphNodePtrNN leaf2=makeNewLeaf( mkAlert("cat", "kszy") );
  s_.process(leaf2, changed_);
  ensure_equals("correlation failed", changed_.size(), 1u);

  ensure_equals("invalid name", changed_[0]->getMetaAlert()->getName().get(), string("[usersmonitor] actions of user 'cat'") );
}

} // namespace tut
