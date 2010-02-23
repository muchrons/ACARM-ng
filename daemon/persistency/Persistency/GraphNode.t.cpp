/*
 * GraphNode.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

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
    ma1_( new MetaAlert( makeNewAlert() ) ),
    ma2_( new MetaAlert( makeNewAlert() ) ),
    conn_(new TestIOConnection),
    t_( conn_->createNewTransaction("gn_test") ),
    leaf_( makeLeaf() ),
    node_( makeNode() )
  {
  }

  GraphNodePtrNN makeNode(int extraNodes=0)
  {
    MetaAlertPtrNN ma( new MetaAlert("meta alert x",
                                     42,
                                     0,
                                     makeNewReferenceURL(),
                                     Timestamp() ) );
    NodeChildrenVector vec( makeLeaf(), makeLeaf() );
    for(int i=0; i<extraNodes; ++i)
      vec.push_back( makeLeaf() );

    return GraphNodePtrNN( new GraphNode(ma, conn_, t_, vec) );
  }

  GraphNodePtrNN makeLeaf(void)
  {
    return GraphNodePtrNN( new GraphNode( makeNewAlert(), conn_, t_) );
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
                                   makeNewReferenceURL(),
                                   Timestamp() ) );
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
    fail("addChild() didn't failed for 1-stop-long cycle");
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

    for(int i=0; i<666; ++i)
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
  boost::thread th1( NodeAdder(this, node_) );
  boost::thread th2( NodeAdder(this, node_) );
  // wait till threads exit.
  th1.join();
  th2.join();
}

// test self-comparison
template<>
template<>
void testObj::test<18>(void)
{
  TestHelpers::checkEquality( *makeNode(), *makeLeaf() );
}

// test comparison with toher, but identical object
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
  GraphNodePtrNN node  =makeNode();
  GraphNodePtrNN child=makeNode();
  node->addChild(child, *makeIO(node) );
  TestHelpers::checkEquality(*node, *makeNode() );
}

} // namespace tut
