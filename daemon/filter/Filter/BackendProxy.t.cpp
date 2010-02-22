/*
 * BackendProxy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Filter/BackendProxy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Filter;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( IO::create() ),
    bp_( new BackendProxy(conn_, changed_, "sometest") )
  {
    assert( bp_.get()!=NULL );
    assert( conn_.get()!=NULL );
    assert( changed_.size()==0 );
  }

  MetaAlertPtrNN makeMetaAlert(void) const
  {
    return makeNewMetaAlert();
  }

  GraphNodePtrNN makeGraphLeaf(void) const
  {
    return makeNewLeaf();
  }
  GraphNodePtrNN makeGraphNode(void) const
  {
    return makeNewNode();
  }

  int childrenCount(const GraphNodePtrNN ptr) const
  {
    int              cnt=0;
    for(GraphNode::const_iterator it=ptr->begin(); it!=ptr->end(); ++it)
      ++cnt;
    return cnt;
  }

  HostPtrNN setName(const char *name)
  {
    const HostPtrNN      h=makeNewHost();
    Alert::ReportedHosts srcHosts;
    srcHosts.push_back(h);
    AlertPtrNN           alert( new Alert("al1",
                                          makeNewAnalyzer(),
                                          NULL,
                                          Timestamp(),
                                          Severity(SeverityLevel::DEBUG),
                                          Certainty(0.1),
                                          "sescription xyz",
                                          srcHosts,
                                          Alert::ReportedHosts() ) );
    Persistency::IO::ConnectionPtrNN conn=Persistency::IO::create();
    IO::Transaction      t( conn->createNewTransaction("make_leaf_transaction") );
    GraphNodePtrNN       node( new GraphNode(alert, conn, t) );
    bp_->setHostName(node, h, name);

    return h;
  }

  BackendProxy::ChangedNodes      changed_;
  IO::ConnectionPtrNN             conn_;
  boost::scoped_ptr<BackendProxy> bp_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/BackendProxy");
} // unnamed namespace


namespace tut
{

// check if d-tor, when nothing's done, works fine
template<>
template<>
void testObj::test<1>(void)
{
  bp_.reset();
}

// check setting name of a host
template<>
template<>
void testObj::test<2>(void)
{
  const std::string name("hello.pl");
  const HostPtrNN   h=setName( name.c_str() );
  ensure("name not set", h->getName()!=NULL );
  ensure_equals("invalid name set", h->getName()->get(), name);
  ensure_equals("change not marked", changed_.size(), 1);
}

// check updating severity
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN node=makeGraphNode();
  MetaAlertPtrNN ma  =node->getMetaAlert();
  const double c=ma->getSeverityDelta();
  bp_->updateSeverityDelta(node, 1.3);
  ensure_equals("invalid severity", ma->getSeverityDelta(), c+1.3);
  ensure_equals("change not marked", changed_.size(), 1);
}

// test updating certanity
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN node=makeGraphNode();
  MetaAlertPtrNN ma  =node->getMetaAlert();
  const double c=ma->getCertaintyDelta();
  bp_->updateCertaintyDelta(node, 1.3);
  ensure_equals("invalid certanity", ma->getCertaintyDelta(), c+1.3);
  ensure_equals("change not marked", changed_.size(), 1);
}

// test commiting empty change-set
template<>
template<>
void testObj::test<5>(void)
{
  bp_->commitChanges();
}

// test commiting some change
template<>
template<>
void testObj::test<6>(void)
{
  setName("a.b.c");
  bp_->commitChanges();
}

// test adding child to graph node, with default extra nodes
template<>
template<>
void testObj::test<7>(void)
{
  GraphNodePtrNN parent=makeGraphNode();
  GraphNodePtrNN child =makeGraphNode();
  bp_->addChild(parent, child);
  // check
  ensure_equals("child not added",    childrenCount(parent), 2+1);
  ensure_equals("wrong way addition", childrenCount(child),  2+0);
  ensure_equals("change not marked", changed_.size(), 1);
}

// test correlating nodes
template<>
template<>
void testObj::test<8>(void)
{
  GraphNodePtrNN leaf1=makeGraphLeaf();
  GraphNodePtrNN node2=makeGraphNode();
  GraphNodePtrNN out  =bp_->correlate( makeMetaAlert(), leaf1, node2);
  // check
  ensure_equals("invalid number of children after correlation",
                childrenCount(out), 2);
  ensure_equals("change not marked", changed_.size(), 1);
}

// test correlating more than 2 hosts.
template<>
template<>
void testObj::test<9>(void)
{
  GraphNodePtrNN leaf1=makeGraphLeaf();
  GraphNodePtrNN node2=makeGraphNode();
  BackendProxy::ChildrenVector children;
  for(int i=0; i<3; ++i)
    children.push_back( makeGraphNode() );
  GraphNodePtrNN out  =bp_->correlate( makeMetaAlert(), leaf1, node2, children);
  // check
  ensure_equals("invalid number of children after correlation",
                childrenCount(out), 2+3);
  ensure_equals("change not marked", changed_.size(), 1);
}

// test throwing when non-mepty changed nodes colleciton is given
template<>
template<>
void testObj::test<10>(void)
{
  try
  {
    changed_.push_back( makeGraphNode() );  // colleciton shall be non-empty
    BackendProxy tmp(conn_, changed_, "myunatedstatesofwhatever");
    fail("c-tor didn't throw on non-empty changed nodes' collection");
  }
  catch(const BackendProxy::ExceptionChangedNodesNotEmpty&)
  {
    // this is expected
  }
}

} // namespace tut
