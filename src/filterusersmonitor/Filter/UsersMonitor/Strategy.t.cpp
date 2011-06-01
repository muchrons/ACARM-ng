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
using namespace Core::Types::Proc;
using namespace Filter::UsersMonitor;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestBase
{
  TestClass(void):
    s_( InstanceName("somename"), Strategy::Parameters(997, Data::Names() ) )
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

// test if skip-lists work properly
template<>
template<>
void testObj::test<5>(void)
{
  Data::Names skip;
  skip.push_back("cat");
  Strategy    s( InstanceName("myname"), Strategy::Parameters(123, skip) );
  GraphNodePtrNN leaf1=makeNewLeaf( mkAlert("cat") );
  s.process(leaf1, changed_);
  ensure_equals("something has been changed", changed_.size(), 0u);

  GraphNodePtrNN leaf2=makeNewLeaf( mkAlert("cat") );
  s.process(leaf2, changed_);
  ensure_equals("correlation doesn't check skip list", changed_.size(), 0u);
}

// test if correlation can proceed when skip-list matches only one pair
template<>
template<>
void testObj::test<6>(void)
{
  Data::Names skip;
  skip.push_back("cat");
  Strategy    s( InstanceName("myname"), Strategy::Parameters(123, skip) );
  GraphNodePtrNN leaf1=makeNewLeaf( mkAlert("cat", "doom") );
  s.process(leaf1, changed_);
  ensure_equals("something has been changed", changed_.size(), 0u);

  GraphNodePtrNN leaf2=makeNewLeaf( mkAlert("cat", "doom") );
  s.process(leaf2, changed_);
  ensure_equals("correlation failed after first skip-match", changed_.size(), 1u);
}

// test correlation of multiple entries
template<>
template<>
void testObj::test<7>(void)
{
  GraphNodePtrNN leaf1=makeNewLeaf( mkAlert("alice", "cat") );
  s_.process(leaf1, changed_);
  ensure_equals("something has been changed", changed_.size(), 0u);

  GraphNodePtrNN leaf2=makeNewLeaf( mkAlert("cat", "kszy") );
  s_.process(leaf2, changed_);
  ensure_equals("correlation 1 failed", changed_.size(), 1u);
  GraphNodePtrNN node=changed_[0];
  ensure("node identical with first one", leaf1.get()!=node.get() );
  ensure("node identical with second one", leaf2.get()!=node.get() );
  changed_.clear();

  GraphNodePtrNN leaf3=makeNewLeaf( mkAlert("cat", "bob") );
  s_.process(leaf3, changed_);
  ensure_equals("correlation 2 failed", changed_.size(), 1u);
  ensure("new node created instead of add to existing one", node.get()==changed_[0].get() );
}

} // namespace tut
