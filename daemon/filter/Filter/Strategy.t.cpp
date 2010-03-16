/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Filter/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestFilter: public Strategy<int>
{
  TestFilter(void):
    Strategy<int>("testfilter"),
    calls_(0),
    node_( makeGraphLeaf() )
  {
  }

  virtual void processImpl(Node               n,
                           NodesTimeoutQueue &ntq,
                           BackendProxy      &bp)
  {
    ++calls_;
    tut::ensure("invalid node", n.get()==node_.get() );
    // smoke test - checks if object is valid
    bp.commitChanges();
    // ntq should be empty by default
    tut::ensure("NTQ not empty", ntq.begin()==ntq.end() );
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
  BackendProxy::ChangedNodes changed_;
  TestFilter                 tf_;
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
  ensure_equals("invalid name", tf_.getFilterName(), "testfilter");
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
    Strategy<char>("testloopfilter")
  {
  }

  virtual void processImpl(Node, NodesTimeoutQueue&, BackendProxy&)
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
  void operator()(void)
  {
    Strategy<char>::ChangedNodes cn;
    tlf_.process( TestFilter::makeGraphLeaf(), cn );
  }

  TestLoopFilter tlf_;
}; // struct CollableLF
} // unnmaed namespace

// check if interruptionPoint() works as expected
template<>
template<>
void testObj::test<3>(void)
{
  CallableLF clf;
  boost::thread th( boost::ref(clf) );
  th.interrupt();
  th.join();
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

} // namespace tut
