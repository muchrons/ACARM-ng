/*
 * GraphNode.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"
#include "TestHelpers/checkEquality.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
  TestClass(void):
    ma1_( new MetaAlert( makeNewAlert(), 42u ) ),
    ma2_( new MetaAlert( makeNewAlert(), 42u ) ),
    conn_(new TestIOConnection),
    t_( conn_->createNewTransaction("gn_test") ),
    leaf_( makeLeaf() ),
    node_( makeNode() )
  {
  }

  GraphNodePtrNN makeNode(int extraNodes=0, unsigned int id=42u)
  {
    MetaAlertPtrNN ma( new MetaAlert("meta alert x",
                                     0,
                                     0,
                                     makeNewReferenceURL().shared_ptr(),
                                     Timestamp(),
                                     id) );
    NodeChildrenVector vec( makeLeaf(id+1u), makeLeaf(id+2u) );
    for(int i=0; i<extraNodes; ++i)
      vec.push_back( makeLeaf(id+3u+i) );

    return GraphNodePtrNN( new GraphNode(ma, conn_, t_, vec) );
  }

  GraphNodePtrNN makeLeaf(unsigned int id=666u)
  {
    return GraphNodePtrNN( new GraphNode( makeNewAlert(), id, conn_, t_) );
  }

  int childrenCount(const GraphNode &gn) const
  {
    int count=0;
    for(GraphNode::const_iterator it=gn.begin(); it!=gn.end(); ++it)
      ++count;
    return count;
  }

  IO::MetaAlertAutoPtr makeIO(GraphNodePtrNN gn)
  {
    return conn_->metaAlert( gn->getMetaAlert(), t_);
  }

  template<typename T>
  void ignore(const T &) const
  {
    // helper to supress compierl warnign about unsued variables.
  }

  MetaAlertPtrNN      ma1_;
  MetaAlertPtrNN      ma2_;
  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;

  GraphNodePtrNN      leaf_;
  GraphNodePtrNN      node_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/GraphNode");
} // unnamed namespace


namespace tut
{

// test creationg from alert
template<>
template<>
void testObj::test<1>(void)
{
  ensure("invalid node type", leaf_->isLeaf() );
  ensure("NULL meta-alert pointer returned", leaf_->getMetaAlert().get()!=NULL );
  ensure("NULL alert pointer returned", leaf_->getAlert().get()!=NULL );
}

// test if leaf throws on begin call
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    leaf_->begin();
    fail("begin() for leaf didn't throw");
  }
  catch(const ExceptionNotNode&)
  {
    // this is expected
  }
}

// test if leaf throws on end call
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    leaf_->end();
    fail("end() for leaf didn't throw");
  }
  catch(const ExceptionNotNode&)
  {
    // this is expected
  }
}

// test throw on const-begin for leaf
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    const GraphNode &gn=*leaf_;
    gn.begin();
    fail("const-begin() for leaf didn't throw");
  }
  catch(const ExceptionNotNode&)
  {
    // this is expected
  }
}

// test const-end throw for leaf
template<>
template<>
void testObj::test<5>(void)
{
  try
  {
    const GraphNode &gn=*leaf_;
    gn.end();
    fail("const-end() for leaf didn't throw");
  }
  catch(const ExceptionNotNode&)
  {
    // this is expected
  }
}

// test throw on addChild for leaf.
template<>
template<>
void testObj::test<6>(void)
{
  try
  {
    leaf_->addChild( makeLeaf(),  *makeIO(leaf_) );
    fail("addChild() for leaf didn't throw");
  }
  catch(const ExceptionNotNode&)
  {
    // this is expected
  }
}

// test basic node functionality
template<>
template<>
void testObj::test<7>(void)
{
  ensure("invalid node type", node_->isLeaf()==false );
  ensure("NULL meta-alert pointer returned", node_->getMetaAlert().get()!=NULL );
}

// test throw from getAlert() when called on non-leaf
template<>
template<>
void testObj::test<8>(void)
{
  try
  {
    node_->getAlert();
    fail("getAlert() didn't failed when called on node");
  }
  catch(const ExceptionNotLeaf&)
  {
    // this is expected
  }
}

// test default paramter with additional nodes
template<>
template<>
void testObj::test<9>(void)
{
  MetaAlertPtrNN ma( new MetaAlert("meta alert x",
                                   42,
                                   0,
                                   makeNewReferenceURL().shared_ptr(),
                                   Timestamp(),
                                   123u ) );
  const NodeChildrenVector vec( makeLeaf(), makeLeaf() );
  const GraphNode          gn(ma, conn_, t_, vec);
  ensure_equals("invalid children count", childrenCount(gn), 2);
}

// test count when empty vector has been given with extra children
template<>
template<>
void testObj::test<10>(void)
{
  GraphNodePtrNN gn=makeNode(0);
  ensure_equals("invalid number of children for empty extra nodes",
                childrenCount(*gn), 2);
}

// test adding extra nodes too
template<>
template<>
void testObj::test<11>(void)
{
  GraphNodePtrNN gn=makeNode(3);
  ensure_equals("invalid number of children for non-empty extra nodes",
                childrenCount(*gn), 5);
}

// test adding a child
template<>
template<>
void testObj::test<12>(void)
{
  ensure_equals("invalid initial size", childrenCount(*node_), 2);
  node_->addChild(leaf_, *makeIO(node_) );
  ensure_equals("addition failed", childrenCount(*node_), 3);
}

// test cycle detection when node itself is given as argument.
template<>
template<>
void testObj::test<13>(void)
{
  try
  {
    node_->addChild(node_, *makeIO(node_) );
    fail("addChild() didn't failed for point-to-self cycle");
  }
  catch(const ExceptionCycleDetected&)
  {
    // this is expected
  }
}

// test cycle child point back to parent
template<>
template<>
void testObj::test<14>(void)
{
  GraphNodePtrNN child=makeNode();
  child->addChild(node_, *makeIO(child) );
  try
  {
    node_->addChild(child, *makeIO(node_) );
    fail("addChild() didn't failed for 1-step-long cycle");
  }
  catch(const ExceptionCycleDetected&)
  {
    // this is expected
  }
}

// test for cycle detection when multiple sub-nodes have to be checked
template<>
template<>
void testObj::test<15>(void)
{
  GraphNodePtrNN child1=makeNode();
  GraphNodePtrNN child2=makeNode();
  GraphNodePtrNN child3=makeNode();

  // child3->{..., node_}
  child1->addChild(node_, *makeIO(child1) );
  // child2->{..., child3}
  child2->addChild(child3, *makeIO(child2) );
  // child1->{..., child2}
  child1->addChild(child2, *makeIO(child1) );
  try
  {
    // node_->{..., child1}
    node_->addChild(child1, *makeIO(node_) );
    fail("addChild() didn't failed while creating loop");
  }
  catch(const ExceptionCycleDetected&)
  {
    // this is expected.
  }
}

// test adding two child, that have common child (non-cycle)
template<>
template<>
void testObj::test<16>(void)
{
  GraphNodePtrNN child1=makeNode();
  GraphNodePtrNN child2=makeNode();
  GraphNodePtrNN child3=makeNode();

  // child2->{..., child1}
  child2->addChild(child1, *makeIO(child2) );
  // child3->{..., child1}
  child3->addChild(child1, *makeIO(child3) );

  // node_->{..., child2}
  node_->addChild(child2, *makeIO(node_) );

  // node_->{..., child3}
  node_->addChild(child3, *makeIO(node_) );
}

namespace
{
struct NodeAdder
{
  NodeAdder(TestClass      *testClass,
            GraphNodePtrNN  node):
    testClass_(testClass),
    gn_(node)
  {
    assert(testClass_!=NULL);
  }

  void operator()(void)
  {
    IO::MetaAlertAutoPtr io=testClass_->makeIO(gn_);

    for(int i=0; i<303; ++i)
    {
      assert( io.get()!=NULL );
      gn_->addChild( testClass_->makeNode(), *io);
    }
  }

  TestClass            *testClass_;
  GraphNodePtrNN        gn_;
}; // struct NodeAdder
} // unnamed namespace

// test writtings from multiple threads
template<>
template<>
void testObj::test<17>(void)
{
  // run writes from 2 threads
  Base::Threads::ThreadJoiner th1( NodeAdder(this, node_) );
  Base::Threads::ThreadJoiner th2( NodeAdder(this, node_) );
  // wait till threads exit.
  th1->join();
  th2->join();
}

// test self-comparison
template<>
template<>
void testObj::test<18>(void)
{
  TestHelpers::checkEquality( *makeNode(), *makeLeaf() );
}

// test comparison with other instances identical object
template<>
template<>
void testObj::test<19>(void)
{
  TestHelpers::checkEquality( *makeNode(), *makeNode(), *makeLeaf() );
}

// test self-comparison directly
template<>
template<>
void testObj::test<20>(void)
{
  ensure( "comparing two instances of the same leaf failed",
          *makeLeaf()==*makeLeaf() );
}

// test comparing nodes that differ in children count
template<>
template<>
void testObj::test<21>(void)
{
  GraphNodePtrNN node =makeNode();
  GraphNodePtrNN child=makeNode();
  node->addChild(child, *makeIO(node) );
  TestHelpers::checkEquality(*node, *makeNode() );
}

// test get meta-alert - const version.
template<>
template<>
void testObj::test<22>(void)
{
  const GraphNodePtrNN  n =makeNode();
  const MetaAlertPtrNN &ma=n->getMetaAlert();   // smoke test
  ignore(ma);
}

// test get alert - const version.
template<>
template<>
void testObj::test<23>(void)
{
  const GraphNodePtrNN  n=makeLeaf();
  const AlertPtrNN     &a=n->getAlert();        // smoke test
  ignore(a);
}

// test get alert on node - const version.
template<>
template<>
void testObj::test<24>(void)
{
  const GraphNodePtrNN n=makeNode();
  ensure("pre-condition failed", !n->isLeaf() );
  try
  {
    n->getAlert();
    fail("getAlert() const - didn't throw on non-leaf object");
  }
  catch(const ExceptionNotLeaf &)
  {
    // this is expected
  }
}


namespace
{
struct BigThread
{
  BigThread(GraphNodePtrNN root, TestClass &tc):
    root_(root),
    big_( tc.makeNode(50) ),
    tc_(&tc)
  {
  }

  void operator()(void)
  {
    root_->addChild(big_, *tc_->makeIO(big_) ); // this should take a while and so
                                                // lock root_ object for some time
  }

private:
  GraphNodePtrNN  root_;
  GraphNodePtrNN  big_;
  TestClass      *tc_;
}; // struct BigThread
} // unnamed namespace

// test if WaitingLockData resets wait-for pointer when error occured
template<>
template<>
void testObj::test<25>(void)
{
  // force error
  try
  {
    node_->addChild(node_, *makeIO(node_) );
    fail("addChild() didn't failed for point-to-self cycle");
  }
  catch(const ExceptionCycleDetected&)
  {
    // this is expected
  }

  GraphNodePtrNN              top=makeNode();
  // run some other addition in separte thread
  BigThread                   bt(node_, *this);
  Base::Threads::ThreadJoiner th(bt);   // start thread
  boost::thread::yield();               // go to thread

  // this call must not throw nor hung
  top->addChild(node_, *makeIO(node_) );

  // join thread
  th->join();
}

// test not-adding when child is not unique
template<>
template<>
void testObj::test<26>(void)
{
  // create parent
  GraphNodePtrNN gn   =makeNode();
  ensure_equals("invalid children count", childrenCount(*gn), 2);
  GraphNodePtrNN child=makeNode();
  // add child for the first time
  gn->addChild(child, *makeIO(child) );
  ensure_equals("child not added", childrenCount(*gn), 3);
  // now try adding second time - nothing should happen
  gn->addChild(child, *makeIO(child) );
  ensure_equals("duplicate child has been added", childrenCount(*gn), 3);
}

// test presence of ID typedef
template<>
template<>
void testObj::test<27>(void)
{
  const GraphNode::ID id(42u);
  ensure_equals("invalid ID", id.get(), 42u);
}

// test comparison when order of children differ
template<>
template<>
void testObj::test<28>(void)
{
  GraphNodePtrNN node1 =makeNode();
  GraphNodePtrNN node2 =makeNode();
  GraphNodePtrNN child1=makeNode(0, 10);
  GraphNodePtrNN child2=makeNode(0, 20);

  node1->addChild(child1, *makeIO(node1) );
  node1->addChild(child2, *makeIO(node1) );

  node2->addChild(child2, *makeIO(node2) );
  node2->addChild(child1, *makeIO(node2) );

  ensure("compariosn of nodes with children in different order failed", *node1==*node2);
}

} // namespace tut
