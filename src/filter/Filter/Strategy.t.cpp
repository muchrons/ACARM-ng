/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Filter/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/delay.hpp"

using namespace Filter;
using namespace Persistency;
using namespace Core::Types::Proc;
using namespace TestHelpers::Persistency;
using TestHelpers::delay;

namespace
{

struct TestFilter: public Strategy<int>
{
  TestFilter(void):
    Strategy<int>( TypeName("testfilter"), InstanceName("testfiltername") ),
    calls_(0),
    node_( makeGraphLeaf() )
  {
  }

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendFacade      &bf)
  {
    ++calls_;
    tut::ensure("invalid node", n.get()==node_.get() );
    // smoke test - checks if object is valid
    bf.commitChanges();
    // ntq should be empty by default
    tut::ensure("NTQ not empty", ntq.begin()==ntq.end() );
    // now add something
    ntq.update( NodeEntry(n, 0), 1 );
  }

  static GraphNodePtrNN makeGraphLeaf(void)
  {
    return makeNewLeaf();
  }

  static NodeEntry makeNodeEntry(Node n, int v)
  {
    return NodeEntry(n, v);
  }

  int  calls_;
  Node node_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  BackendFacade::ChangedNodes changed_;
  TestFilter                  tf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", tf_.getFilterType().str(), "testfilter");
}

// check if process() calls implementation (with valid arguments)
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-condition failed", tf_.calls_, 0);
  tf_.process(tf_.node_, changed_);
  ensure_equals("invalid number of calls", tf_.calls_, 1);
}


namespace
{
struct TestLoopFilter: public Strategy<char>
{
  TestLoopFilter(void):
    Strategy<char>( TypeName("testloopfilter"), InstanceName("testloopfiltername") )
  {
  }

  virtual void processImpl(Node, NodesTimeoutQueue&, BackendFacade&)
  {
    for(;;)
    {
      boost::this_thread::yield();  // avoid too much CPU waste
      interruptionPoint();
    }
  }
}; // struct TestLoopFilter

struct CallableLF
{
  CallableLF(TestLoopFilter *tlf):
    tlf_(tlf)
  {
  }

  void operator()(void)
  {
    Strategy<char>::ChangedNodes cn;
    tlf_->process( TestFilter::makeGraphLeaf(), cn );
  }

  TestLoopFilter *tlf_;
}; // struct CollableLF
} // unnmaed namespace

// check if interruptionPoint() works as expected
template<>
template<>
void testObj::test<3>(void)
{
  TestLoopFilter tlf;
  Base::Threads::ThreadJoiner th( (CallableLF(&tlf)) );
  th->interrupt();
  th->join();
}

// check if NodeEntry with different nodes differ template<>
template<>
template<>
void testObj::test<4>(void)
{
  ensure("different entries does not differ",
           ! ( TestFilter::makeNodeEntry( TestFilter::makeGraphLeaf(), 1 )==
               TestFilter::makeNodeEntry( TestFilter::makeGraphLeaf(), 1 ) )
        );
}

// test if creating NodeEntry assigns proper node.
template<>
template<>
void testObj::test<5>(void)
{
  TestFilter::Node n=TestFilter::makeGraphLeaf();
  ensure("invalid node", TestFilter::makeNodeEntry(n, 42).node_.get()==n.get() );
}

// test if creating NodeEntry assigns proper paramter.
template<>
template<>
void testObj::test<6>(void)
{
  TestFilter::Node n=TestFilter::makeGraphLeaf();
  ensure_equals("invalid value",
                TestFilter::makeNodeEntry(n, 42).t_, 42 );
}

// test operator != for different nodes
template<>
template<>
void testObj::test<7>(void)
{
  ensure("different entries does not differ",
           TestFilter::makeNodeEntry( TestFilter::makeGraphLeaf(), 1 )!=
           TestFilter::makeNodeEntry( TestFilter::makeGraphLeaf(), 1 )
        );
}

// test operator != for the same nodes
template<>
template<>
void testObj::test<8>(void)
{
  TestFilter::Node n=TestFilter::makeGraphLeaf();
  ensure("the same entries differ for != operator",
          !( TestFilter::makeNodeEntry(n, 1)!=TestFilter::makeNodeEntry(n, 1) )
        );
}

// check operator == for the same nodes
template<>
template<>
void testObj::test<9>(void)
{
  TestFilter::Node n=TestFilter::makeGraphLeaf();
  ensure("the same entruies differ for == operator",
         TestFilter::makeNodeEntry(n, 1)==TestFilter::makeNodeEntry(n, 1) );
}

// ensure different user-paramter does noe influence comparison
template<>
template<>
void testObj::test<10>(void)
{
  TestFilter::Node n=TestFilter::makeGraphLeaf();
  ensure("user-value is taken into account in comparison",
         TestFilter::makeNodeEntry(n, 1)==TestFilter::makeNodeEntry(n, 2) );
}

// test timeouting and cleanup after that
template<>
template<>
void testObj::test<11>(void)
{
  // prepare data and save weak pointer to it
  boost::weak_ptr<TestFilter::Node::value_type> weakNode( tf_.node_.shared_ptr() );
  ensure("weak pointer is invalid", weakNode.lock().get()==tf_.node_.get() );
  // create and run strategy that will save this node inside
  {
    TestFilter::ChangedNodes changed;
    tf_.process(tf_.node_, changed);        // save it internally for later correlation
  }
  tf_.node_=makeNewLeaf();                  // remove old pointer (by overwiriting, since it cannot be NULL)
  ensure("node has not been saved - probably bug in a test case", weakNode.lock().get()!=NULL );
  delay(2);                                 // timeout for tests
  tf_.process(tf_.node_, changed_);         // run process() again to remove timeouted entries
  ensure("node has not been removed - queue's not cleared", weakNode.lock().get()==NULL );
}

// sending heartbeat smoke test
template<>
template<>
void testObj::test<12>(void)
{
  tf_.heartbeat(42u);
}

} // namespace tut
