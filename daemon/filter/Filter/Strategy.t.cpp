/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Filter/Strategy.hpp"
#include "Filter/TestHelpers.t.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter;
using namespace Persistency;

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

  static MetaAlertPtrNN makeMetaAlert(void)
  {
    return th_makeMetaAlert();
  }

  static GraphNodePtrNN makeGraphLeaf(void)
  {
    return th_makeLeaf();
  }

  int  calls_;
  Node node_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  BackendProxy::ChangedNodes changed_;
  TestFilter                 tf_;
};

typedef TestClass TestClass;
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

} // namespace tut
