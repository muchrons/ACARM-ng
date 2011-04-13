/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/SameName/Strategy.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::SameName;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
  TestClass(void):
    sampleLeaf_( makeNewLeaf( makeNewAlert("some alert") ) ),
    s_("somename", 997)
  {
  }
  GraphNodePtrNN         sampleLeaf_;
  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/SameName/Strategy");
} // unnamed namespace

namespace tut
{


// test correlation of two alerts
template<>
template<>
void testObj::test<1>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlert("some alert") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
}

// test if can correltate will return false for defferent names
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlert("some other alert") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);
}

// test getting name of meta alert
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlert("some alert") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
  const string resp("[samename] some alert");
  ensure_equals("invalid name",
                changed_[0]->getMetaAlert()->getName().get(), resp);
}

// test correlation of three alerts
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN tmp( makeNewLeaf( makeNewAlert("some alert") ) );
  s_.process(tmp, changed_);
  ensure_equals("some nodes have been changed", changed_.size(), 0u);

  s_.process(sampleLeaf_, changed_);
  ensure_equals("no nodes changed", changed_.size(), 1u);
  GraphNodePtrNN correlated=changed_.at(0);
  changed_.clear();

  tmp=makeNewLeaf( makeNewAlert("some alert") );
  s_.process(tmp, changed_);
  ensure_equals("no nodes changed - 3rd alerts' correaltion failed", changed_.size(), 1u);
  ensure("correlation not added to existing node", correlated.get()==changed_.at(0).get() );
}

} // namespace tut
