/*
 * Processor.t.cpp
 *
 */
#include <tut.h>

#include "Filter/Processor.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/Stubs/TestHelpers.hpp"

using namespace Filter;
using namespace Persistency;
using namespace Persistency::Stubs;

namespace
{

struct TestFilter: public Interface
{
  TestFilter(void):
    Interface("testfilter"),
    calls_(0),
    node_( makeGraphLeaf() )
  {
  }

  virtual void processImpl(Node               n,
                           ChangedNodes      &changed,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp)
  {
    ++calls_;
    tut::ensure_equals("invalid count of elements in changed list",
                       changed.size(), 0);

    tut::ensure("invalid node", n.get()==node_.get() );
    tut::ensure("invalid changes list", &changed==&changed_);

    // smoke test - checks if object is valid
    bp.commitChanges();

    // node and changed should be writable
    changed.push_back( makeGraphLeaf() );
    changed.push_back(n);
    n->getMetaAlert();

    // ntq should be empty by default
    tut::ensure("NTQ not empty", ntq.begin()==ntq.end() );
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

  int          calls_;
  ChangedNodes changed_;
  Node       node_;
};


struct TestClass
{
  TestClass(void):
    filter_(new TestFilter)
  {
  }

  Core::Types::NodesFifo      mainQueue_;
  Processor::InterfaceAutoPtr filter_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Processor");
} // unnamed namespace


namespace tut
{

// test d-tor when nothing has been called (thread should be aborted)
template<>
template<>
void testObj::test<1>(void)
{
  Processor p(mainQueue_, filter_);
  usleep(30*1000);                      // wait a while to ensure thread is running
  // when exiting this should not block
}

// test c-tor throw when NULL filter passed
template<>
template<>
void testObj::test<2>(void)
{
  Processor::InterfaceAutoPtr tmp;
  ensure("pre-condition failed", tmp.get()==NULL );
  try
  {
    Processor p(mainQueue_, tmp);
    fail("processor's c-tor didn't throw on NULL filter");
  }
  catch(const ExceptionInvalidFilter &)
  {
    // this is expected
  }
}

// 
template<>
template<>
void testObj::test<3>(void)
{
}

// 
template<>
template<>
void testObj::test<4>(void)
{
}

// 
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
