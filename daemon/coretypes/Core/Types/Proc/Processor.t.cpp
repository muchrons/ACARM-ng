/*
 * Processor.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "Core/Types/Proc/Processor.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core::Types::Proc;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestInterface: public Interface
{
  TestInterface(void):
    Interface("testinterface"),
    calls_(0),
    node_( makeNewLeaf() ),
    node2_( makeNewLeaf() )
  {
    assert( node_.get()!=node2_.get() );
  }

  virtual void process(Node node, ChangedNodes &changedNodes)
  {
    ++calls_;

    tut::ensure("invalid node", node.get()==node_.get() ||
                                node.get()==node2_.get() );
    tut::ensure_equals("invalid count of elements in changed list",
                       changedNodes.size(), 0u);

    // add two nodes to chenged list
    changedNodes.push_back(node);
  }

  int  calls_;
  Node node_;
  Node node2_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    interface_(new TestInterface)
  {
  }

  Core::Types::NodesFifo      mainQueue_;
  Processor::InterfaceAutoPtr interface_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/Processor");
} // unnamed namespace


namespace tut
{

// test d-tor when nothing has been called (thread should be aborted)
template<>
template<>
void testObj::test<1>(void)
{
  Processor p(mainQueue_, interface_);
  usleep(30*1000);                      // wait a while to ensure thread is running
  // when exiting this should not block
}

// test c-tor throw when NULL interface passed
template<>
template<>
void testObj::test<2>(void)
{
  Processor::InterfaceAutoPtr tmp;
  ensure("pre-condition failed", tmp.get()==NULL );
  try
  {
    Processor p(mainQueue_, tmp);
    fail("processor's c-tor didn't throw on NULL interface");
  }
  catch(const ExceptionInvalidInterface&)
  {
    // this is expected
  }
}

// test whole call sequence
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("pre-condition failed", mainQueue_.size(), 0u);
  // process data
  TestInterface &ti=dynamic_cast<TestInterface&>(*interface_);
  Processor      p(mainQueue_, interface_);
  p.process(ti.node_);  // this call should add 1 element to 'changed'
                        // elements set, and processor should forward it
                        // to the main queue.
  // wait for the results
  for(int i=0; i<10; ++i)
    if( mainQueue_.size()==1 )
      break;
    else
      usleep(50*1000);
  // check results
  ensure_equals("invalid number of elements", mainQueue_.size(), 1u);
  ensure("requested element not found", mainQueue_.pop().get()==ti.node_.get() );
}

// test if multiple calls to process() does not break anything
// (i.e. if 'changed' set is cleared before each call)
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("pre-condition failed", mainQueue_.size(), 0u);
  // process data
  TestInterface &ti=dynamic_cast<TestInterface&>(*interface_);
  Processor      p(mainQueue_, interface_);
  p.process(ti.node_);          // this call should add 2 elements to 'changed'
  p.process(ti.node2_);         // elements set, and processor should forward it
                                // to the main queue.
  // wait for the results
  for(int i=0; i<10; ++i)
    if( mainQueue_.size()==2 )
      break;
    else
      usleep(50*1000);
  // check results
  ensure_equals("invalid number of elements", mainQueue_.size(), 2u);
}

// smoke-test smart-poitner declaration
template<>
template<>
void testObj::test<5>(void)
{
  ProcessorPtrNN p( new Processor(mainQueue_, interface_) );
  ensure("NULL pointer received", p.get()!=NULL );  // could be assert as well
}

} // namespace tut
