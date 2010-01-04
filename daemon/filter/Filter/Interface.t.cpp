/*
 * Interface.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "Filter/Interface.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/Stubs/TestHelpers.hpp"

using namespace Filter;
using namespace Persistency;
using namespace Persistency::Stubs;

namespace
{

struct TestClass: protected Interface
{
  TestClass(void):
    Interface("test_filter"),
    calls_(0),
    node_( makeGraphLeaf() )
  {
  }

  virtual void processImpl(Node n, NodesDeque &changed, BackendProxy &bp)
  {
    ++calls_;
    tut::ensure_equals("invalid count of elements in changed list",
                       changed.size(), 0);

    tut::ensure("invalid node", n.get()==node_.get() );
    tut::ensure("invalid changes list", &changed==&changed_);
    // smoke test - checks if object is valid
    bp.commitChanges();
  }

  MetaAlertPtrNN makeMetaAlert(void) const
  {
    return MetaAlertPtrNN( new MetaAlert( makeNewAlert() ) );
  }

  GraphNodePtrNN makeGraphLeaf(void)
  {
    Persistency::IO::ConnectionPtrNN conn=Persistency::IO::create();
    const IO::Transaction t( conn->createNewTransaction("graph_transaction") );
    return GraphNodePtrNN( new GraphNode( makeNewAlert(), conn,  t) );
  }

  int        calls_;
  NodesDeque changed_;
  Node       node_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Interface");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", getFilterName(), "test_filter");
}

// check if process() calls implementation (with valid arguments)
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-condition failed", calls_, 0);
  process(node_, changed_);
  ensure_equals("invalid number of calls", calls_, 1);
}

} // namespace tut
