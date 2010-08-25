/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Filter/BackendFacade.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/ConnectionIOCounter.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Filter;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( new ConnectionIOCounter("Persistency::IDAssigner", "next free MetaAlert's ID") ),
    bf_( new BackendFacade(conn_, changed_, "sometest") )
  {
    assert( bf_.get()!=NULL );
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

  HostPtrNN makeHost(void) const
  {
    return HostPtr( new Host( Host::IPv4::from_string("1.2.3.4"),
                              NULL,
                              "os1",
                              makeNewReferenceURL(),
                              Host::ReportedServices(),
                              Host::ReportedProcesses(),
                              NULL) );
  }

  HostPtrNN setName(const char *name)
  {
    const HostPtrNN      h=makeHost();
    Alert::ReportedHosts srcHosts;
    srcHosts.push_back(h);
    AlertPtrNN           alert( new Alert("al1",
                                          Alert::SourceAnalyzers( makeNewAnalyzer() ),
                                          NULL,
                                          Timestamp(),
                                          Severity(SeverityLevel::DEBUG),
                                          Certainty(0.1),
                                          "sescription xyz",
                                          srcHosts,
                                          Alert::ReportedHosts() ) );
    Persistency::IO::ConnectionPtrNN conn( Persistency::IO::create() );
    IO::Transaction      t( conn->createNewTransaction("make_leaf_transaction") );
    GraphNodePtrNN       node( new GraphNode(alert, 11u, conn, t) );
    bf_->setHostName(node, h, name);

    return h;
  }

  BackendFacade::ChangedNodes      changed_;
  IO::ConnectionPtrNN              conn_;
  boost::scoped_ptr<BackendFacade> bf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/BackendFacade");
} // unnamed namespace


namespace tut
{

// check if d-tor, when nothing's done, works fine
template<>
template<>
void testObj::test<1>(void)
{
  bf_.reset();
}

// check setting name of a host
template<>
template<>
void testObj::test<2>(void)
{
  const std::string name("hello.pl");
  const HostPtrNN   h=setName( name.c_str() );
  ensure("name not set", h->getName()!=NULL );
  ensure_equals("invalid name set", h->getName().get(), name);
  ensure_equals("change not marked", changed_.size(), 1u);
}

// check updating severity
template<>
template<>
void testObj::test<3>(void)
{
  GraphNodePtrNN node=makeGraphNode();
  MetaAlertPtrNN ma  =node->getMetaAlert();
  const double c=ma->getSeverityDelta();
  bf_->updateSeverityDelta(node, 1.3);
  ensure_equals("invalid severity", ma->getSeverityDelta(), c+1.3);
  ensure_equals("change not marked", changed_.size(), 1u);
}

// test updating certanity
template<>
template<>
void testObj::test<4>(void)
{
  GraphNodePtrNN node=makeGraphNode();
  MetaAlertPtrNN ma  =node->getMetaAlert();
  const double c=ma->getCertaintyDelta();
  bf_->updateCertaintyDelta(node, 1.3);
  ensure_equals("invalid certanity", ma->getCertaintyDelta(), c+1.3);
  ensure_equals("change not marked", changed_.size(), 1u);
}

// test commiting empty change-set
template<>
template<>
void testObj::test<5>(void)
{
  bf_->commitChanges();
}

// test commiting some change
template<>
template<>
void testObj::test<6>(void)
{
  setName("a.b.c");
  bf_->commitChanges();
}

// test adding child to graph node, with default extra nodes
template<>
template<>
void testObj::test<7>(void)
{
  GraphNodePtrNN parent=makeGraphNode();
  GraphNodePtrNN child =makeGraphNode();
  bf_->addChild(parent, child);
  // check
  ensure_equals("child not added",    childrenCount(parent), 2+1);
  ensure_equals("wrong way addition", childrenCount(child),  2+0);
  ensure_equals("change not marked", changed_.size(), 1u);
}

// test correlating nodes
template<>
template<>
void testObj::test<8>(void)
{
  GraphNodePtrNN                leaf1=makeGraphLeaf();
  GraphNodePtrNN                node2=makeGraphNode();
  BackendFacade::ChildrenVector children(leaf1, node2);
  GraphNodePtrNN                out=bf_->correlate( makeMetaAlert(), children);
  // check
  ensure_equals("invalid number of children after correlation",
                childrenCount(out), 2);
  ensure_equals("change not marked", changed_.size(), 1u);
}

// test correlating more than 2 hosts.
template<>
template<>
void testObj::test<9>(void)
{
  GraphNodePtrNN                leaf1=makeGraphLeaf();
  GraphNodePtrNN                node2=makeGraphNode();
  BackendFacade::ChildrenVector children(leaf1, node2);
  for(int i=0; i<3; ++i)
    children.push_back( makeGraphNode() );
  GraphNodePtrNN out=bf_->correlate( makeMetaAlert(), children);
  // check
  ensure_equals("invalid number of children after correlation",
                childrenCount(out), 2+3);
  ensure_equals("change not marked", changed_.size(), 1u);
}

// test throwing when non-mepty changed nodes colleciton is given
template<>
template<>
void testObj::test<10>(void)
{
  try
  {
    changed_.push_back( makeGraphNode() );  // colleciton shall be non-empty
    BackendFacade tmp(conn_, changed_, "myunatedstatesofwhatever");
    fail("c-tor didn't throw on non-empty changed nodes' collection");
  }
  catch(const BackendFacade::ExceptionChangedNodesNotEmpty&)
  {
    // this is expected
  }
}

// test assigning new IDs
template<>
template<>
void testObj::test<11>(void)
{
  ensure_equals("invalid ID assigned for element 1", bf_->getNextFreeID().get(), 0u);
  ensure_equals("invalid ID assigned for element 2", bf_->getNextFreeID().get(), 1u);
}

} // namespace tut
