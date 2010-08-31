/*
 * NodeChildrenVector.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Persistency/NodeChildrenVector.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;

namespace
{

struct TestClass: private TestBase
{
  TestClass(void):
    conn_(new TestIOConnection),
    t_( conn_->createNewTransaction("gn_test") ),
    leaf1_( makeLeaf() ),
    leaf2_( makeLeaf() ),
    leaf3_( makeLeaf() )
  {
  }

  GraphNodePtrNN makeLeaf(void)
  {
    return GraphNodePtrNN( new GraphNode( makeNewAlert(), 42u, conn_, t_) );
  }

  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;

  GraphNodePtrNN      leaf1_;
  GraphNodePtrNN      leaf2_;
  GraphNodePtrNN      leaf3_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/NodeChildrenVector");
} // unnamed namespace


namespace tut
{

// test creatign sample object
template<>
template<>
void testObj::test<1>(void)
{
  const NodeChildrenVector ncv(leaf1_, leaf2_);
  ensure_equals("invalid size", ncv.size(), 2u);
}

// test adding 3rd element
template<>
template<>
void testObj::test<2>(void)
{
  NodeChildrenVector ncv(leaf1_, leaf2_);
  ensure_equals("invalid size", ncv.size(), 2u);
  ncv.push_back(leaf3_);
  ensure_equals("addition failed", ncv.size(), 3u);
}

} // namespace tut
