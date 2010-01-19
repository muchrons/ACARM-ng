/*
 * BackendProxy.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Filter/BackendProxy.hpp"
#include "Filter/TestHelpers.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "Persistency/IO/BackendFactory.hpp"

using namespace Filter;
using namespace Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    conn_( IO::create() ),
    bp_( new BackendProxy(conn_, "sometest") )
  {
    assert( bp_.get()!=NULL );
    assert( conn_.get()!=NULL );
  }

  MetaAlertPtrNN makeMetaAlert(void) const
  {
    return th_makeMetaAlert();
  }

  GraphNodePtrNN makeGraphLeaf(void) const
  {
    return th_makeLeaf();
  }
  GraphNodePtrNN makeGraphNode(void) const
  {
    return th_makeNode();
  }

  int childrenCount(const GraphNodePtrNN ptr) const
  {
    int              cnt=0;
    for(GraphNode::const_iterator it=ptr->begin(); it!=ptr->end(); ++it)
      ++cnt;
    return cnt;
  }

  IO::ConnectionPtrNN                 conn_;
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
  HostPtrNN         h( makeNewHost() );
  bp_->setHostName(h, name);
  ensure("name not set", h->getName()!=NULL );
  ensure_equals("invalid name set", h->getName()->get(), name);
}

// check updating severity
template<>
template<>
void testObj::test<3>(void)
{
  MetaAlertPtrNN ma( makeMetaAlert() );
  const double c=ma->getSeverityDelta();
  bp_->updateSeverityDelta(ma, 1.3);
  ensure_equals("invalid severity", ma->getSeverityDelta(), c+1.3);
}

// test updating certanity
template<>
template<>
void testObj::test<4>(void)
{
  MetaAlertPtrNN ma( makeMetaAlert() );
  const double c=ma->getCertaintyDelta();
  bp_->updateCertaintyDelta(ma, 1.3);
  ensure_equals("invalid certanity", ma->getCertaintyDelta(), c+1.3);
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
  bp_->setHostName( makeNewHost(), "a.b.c");
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
}

} // namespace tut
