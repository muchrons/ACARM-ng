/*
 * Processor.t.cpp
 *
 */
#include <tut.h>

#include "Filter/Processor.hpp"
#include "Filter/TestHelpers.t.hpp"

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
                           NodesTimeoutQueue &,
                           BackendProxy      &)
  {
    ++calls_;
    tut::ensure_equals("invalid count of elements in changed list",
                       changed.size(), 0);

    // add two nodes to chenged list
    changed.push_back( makeGraphLeaf() );
    changed.push_back(n);
  }

  static MetaAlertPtrNN makeMetaAlert(void)
  {
    return th_makeMetaAlert();
  }

  static GraphNodePtrNN makeGraphLeaf(void)
  {
    return th_makeLeaf();
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

// test whole call sequence
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("pre-condition failed", mainQueue_.size(), 0);
  // process data
  Processor                   p(mainQueue_, filter_);
  Persistency::GraphNodePtrNN node( TestFilter::makeGraphLeaf() );
  p.process(node);              // this call should add 2 elements to 'changed'
                                // elements set, and processor should forward it
                                // to the main queue.
  // wait for the results
  for(int i=0; i<10; ++i)
    if( mainQueue_.size()==2 )
      break;
    else
      usleep(50*1000);
  // check results
  ensure_equals("pre-condition failed", mainQueue_.size(), 2);
  ensure("requested element not found", mainQueue_.pop().get()==node.get() ||
                                        mainQueue_.pop().get()==node.get()   );
}

// test if multiple calls to process() does not break anything
// (i.e. if 'changed' set is cleared before each call)
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("pre-condition failed", mainQueue_.size(), 0);
  // process data
  Processor                   p(mainQueue_, filter_);
  Persistency::GraphNodePtrNN node( TestFilter::makeGraphLeaf() );
  p.process(node);              // this call should add 2 elements to 'changed'
  p.process(node);              // elements set, and processor should forward it
                                // to the main queue.
  // wait for the results
  for(int i=0; i<10; ++i)
    if( mainQueue_.size()==4 )
      break;
    else
      usleep(50*1000);
  // check results
  ensure_equals("pre-condition failed", mainQueue_.size(), 4);
}

} // namespace tut
