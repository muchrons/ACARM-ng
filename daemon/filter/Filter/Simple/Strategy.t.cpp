/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/Simple/Strategy.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::Simple;
using namespace TestHelpers::Persistency;

namespace
{

struct TestData
{
}; // struct TestData

struct TestStrategy: public Strategy<TestData>
{
  TestStrategy(bool isInteresting=true,
               bool canCorrelate =true,
               int  skipCorrelations=0):
    Strategy<TestData>("teststrategy", "teststrategyname", 42),
    isInteresting_(isInteresting),
    canCorrelate_(canCorrelate),
    skipCorrelations_(skipCorrelations),
    postProcessCalled_(0)
  {
  }

  virtual TestData makeThisEntryUserData(const Node /*n*/) const
  {
    return TestData();
  }

  virtual bool isEntryInteresting(const NodeEntry /*thisEntry*/) const
  {
    return isInteresting_;
  }

  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                const NodeEntry /*thisEntry*/,
                                const NodeEntry /*otherEntry*/) const
  {
    return Persistency::MetaAlert::Name("hakuna matata");
  }

  virtual bool canCorrelate(const NodeEntry /*thisEntry*/,
                            const NodeEntry /*otherEntry*/) const
  {
    if( skipCorrelations_-- > 0 )
      return false;
    return canCorrelate_;
  }

  virtual TestData makeUserDataForNewNode(const NodeEntry &/*thisEntry*/,
                                          const NodeEntry &/*otherEntry*/,
                                          const Node       /*newNode*/) const
  {
    return TestData();
  }

  virtual void postProcessNode(Node &/*n*/) const
  {
    ++postProcessCalled_;
  }

  bool        isInteresting_;
  bool        canCorrelate_;
  mutable int skipCorrelations_;
  mutable int postProcessCalled_;
}; // struct TestStrategy



struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestStrategy::ChangedNodes changed_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Simple/Strategy");
} // unnamed namespace


namespace tut
{

// test runs when hosts differ
template<>
template<>
void testObj::test<1>(void)
{
  TestStrategy ts(true, false);
  for(int i=0; i<3; ++i)
  {
    ts.process( makeNewLeaf(), changed_ );
    ensure_equals("something has cahnged", changed_.size(), 0u);
  }
}

// test running when self-entry has been found
template<>
template<>
void testObj::test<2>(void)
{
  TestStrategy       ts;
  TestStrategy::Node n=makeNewLeaf();
  for(int i=0; i<2; ++i)
  {
    ts.process(n, changed_);
    ensure_equals("some nodes have been changed", changed_.size(), 0u);
  }
}

// tests if hosts, that has been marked as interesting are stored.
template<>
template<>
void testObj::test<3>(void)
{
  TestStrategy       ts(true, false);
  TestStrategy::Node n1=makeNewLeaf();
  assert( n1->isLeaf() );
  TestStrategy::Node n2=makeNewLeaf();
  assert( n2->isLeaf() );
  TestStrategy::Node n3=makeNewLeaf();
  assert( n3->isLeaf() );

  ts.process(n1, changed_);
  ensure_equals("some nodes have been changed in first run", changed_.size(), 0u);

  ts.process(n2, changed_);
  ensure_equals("some nodes have been changed in second run", changed_.size(), 0u);

  ts.canCorrelate_=true;
  ts.process(n3, changed_);
  ensure_equals("correlation failed", changed_.size(), 1u);
  // check if changed element is new one indeed
  ensure("node 1 returned", n1.get()!=changed_.at(0).get() );
  ensure("node 2 returned", n2.get()!=changed_.at(0).get() );
  ensure("node 3 returned", n3.get()!=changed_.at(0).get() );

  // check if proper one changed
  ensure("invalid leaf returned", !changed_.at(0)->isLeaf() );

  // check check if children are valid
  GraphNode::const_iterator it =changed_.at(0)->begin();
  GraphNode::const_iterator end=changed_.at(0)->end();
  ensure("no children (?!) present", it!=end);
  ensure("first child not from collection", it->get()==n1.get() );
  ++it;
  ensure("only one child present", it!=end);
  ensure("second child not from paramter", it->get()==n3.get() );
  ++it;
  ensure("too many children", it==end);
}

// correlate new node, when not owning one
template<>
template<>
void testObj::test<4>(void)
{
  TestStrategy       ts;
  TestStrategy::Node n1=makeNewNode();
  TestStrategy::Node n2=makeNewLeaf();

  ts.process(n1, changed_);
  ensure_equals("some nodes have been changed in first run", changed_.size(), 0u);

  ts.process(n2, changed_);
  ensure_equals("correlation new node failed", changed_.size(), 1u);

  // check if changed node is new one
  ensure("node1 returned instead of newly correlated one", n1.get()!=changed_.at(0).get() );
}

namespace
{
struct ThrowStrategy: public TestStrategy
{
  ThrowStrategy(void):
    TestStrategy(true, true, 1),
    throwCount_(3)
  {
  }

  virtual Persistency::MetaAlert::Name getMetaAlertName(
                                      const NodeEntry /*thisEntry*/,
                                      const NodeEntry /*otherEntry*/) const

  {
    if( throwCount_-- > 0 )
      throw Filter::Exception(SYSTEM_SAVE_LOCATION, "just", "testing");
    return "alicehasacat";  // return something
  }

  mutable int throwCount_;
}; // struct ThrowStrategy
} // unnamed namespace

// test if exceptions inside the loop does not break anything
template<>
template<>
void testObj::test<5>(void)
{
  ThrowStrategy ts;
  ts.throwCount_=99999999;      // just to be sure :)
  // add two elements
  for(int i=0; i<2; ++i)
  {
    ts.process( makeNewLeaf(), changed_ );
    ensure_equals("some nodes have been changed / 1", changed_.size(), 0u);
  }

  // nothing should happen here, since two exceptions are thrown
  ts.process( makeNewLeaf(), changed_ );
  ensure_equals("some nodes have been changed / 2", changed_.size(), 0u);

  // this time exception should be thrown only for the first element, it should
  // be logged and then next element should be correlated.
  ts.throwCount_=1;
  ts.process( makeNewLeaf(), changed_ );
  ensure_equals("correlation failed", changed_.size(), 1u);
}

// test if name is set properly
template<>
template<>
void testObj::test<6>(void)
{
  TestStrategy ts;
  ts.process( makeNewLeaf(), changed_ );
  ts.process( makeNewLeaf(), changed_ );
  ensure_equals("nothing changed (no correlation has been done)",
                changed_.size(), 1u);
  ensure_equals( "invalid meta-alert name",
                 string( changed_.at(0)->getMetaAlert()->getName().get() ),
                 string( "hakuna matata" ) );
}

// test run when entries are not interesting
template<>
template<>
void testObj::test<7>(void)
{
  TestStrategy ts(false);
  for(int i=0; i<3; ++i)
  {
    ts.process( makeNewLeaf(), changed_ );
    ensure_equals("something has cahnged", changed_.size(), 0u);
  }
}

// correlate to already exisitng node
template<>
template<>
void testObj::test<8>(void)
{
  TestStrategy       ts;
  TestStrategy::Node n1=makeNewNode();
  TestStrategy::Node n2=makeNewLeaf();
  TestStrategy::Node n3=makeNewNode();

  ts.process(n1, changed_);
  ensure_equals("some nodes have been changed in first run", changed_.size(), 0u);

  ts.process(n2, changed_);
  ensure_equals("correlation failed", changed_.size(), 1u);
  TestStrategy::Node tmp=changed_.at(0);

  changed_.clear();
  ts.process(n3, changed_);
  ensure_equals("correlation to alrady existing node failed", changed_.size(), 1u);

  // check if changed node is new one
  ensure("node1 returned instead of newly correlated one", tmp.get()==changed_.at(0).get() );
}

// test if post-process is called for a new node
template<>
template<>
void testObj::test<9>(void)
{
  TestStrategy       ts;
  TestStrategy::Node n1=makeNewNode();
  TestStrategy::Node n2=makeNewLeaf();

  ts.process(n1, changed_);
  ensure_equals("some nodes have been changed in first run", changed_.size(), 0u);

  ts.process(n2, changed_);
  ensure_equals("correlation new node failed", changed_.size(), 1u);
  ensure_equals("node postprocessing not called", ts.postProcessCalled_, 1);
}

// test if post-process is called for a already correlated node
template<>
template<>
void testObj::test<10>(void)
{
  TestStrategy       ts;
  TestStrategy::Node n1=makeNewNode();
  TestStrategy::Node n2=makeNewLeaf();
  TestStrategy::Node n3=makeNewLeaf();

  ts.process(n1, changed_);
  ensure_equals("some nodes have been changed in first run", changed_.size(), 0u);

  ts.process(n2, changed_);
  ensure_equals("correlation new node failed", changed_.size(), 1u);
  TestStrategy::Node node=changed_[0];
  changed_.clear();

  ts.process(n3, changed_);
  ensure_equals("correlation failed", changed_.size(), 1u);
  ensure("correlation to existing node failed", node.get()==changed_[0].get() );

  // first call to postprocessing has been done during previous correlation
  ensure_equals("node postprocessing not called", ts.postProcessCalled_, 2);
}

// test if post-processing is not called when no corelation happens
template<>
template<>
void testObj::test<11>(void)
{
  TestStrategy ts(false);
  for(int i=0; i<3; ++i)
  {
    ts.process( makeNewLeaf(), changed_ );
    ensure_equals("something has cahnged", changed_.size(), 0u);
    ensure_equals("post processing called when no correlation's done", ts.postProcessCalled_, 0);
  }
}

} // namespace tut
