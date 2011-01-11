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
using Core::Types::SignedNode;

namespace
{

struct TestInterface: public Interface
{
  explicit TestInterface(const EntryControlList &ecl=EntryControlList::createDefaultAccept() ):
    Interface("testinterfacetype", "testinterfacename", ecl),
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

  void waitForResponse(void)
  {
    // wait for the results up to 2[s]
    for(int i=0; i<200; ++i)
      if( mainQueue_.size() > 0 )
        return;
      else
        usleep(10*1000);
    tut::fail("timed out while waiting for response from processor");
  }

  void checkECL(const EntryControlList &ecl, const std::string &caller, bool shouldPass)
  {
    TestInterface *ti=new TestInterface(ecl);
    interface_.reset(ti);
    ProcessorPtrNN p( new Processor(mainQueue_, interface_) );
    p->process( SignedNode(ti->node_, caller) );    // this call should add 1 element to 'changed'
    if(shouldPass)                                  // should we wait for entry?
      waitForResponse();                            // wait for other thread
    else
      usleep(150*1000);                             // do not wait too long if entry is NOT to come
    // check response:
    tut::ensure_equals("invalid collection size", mainQueue_.size(), shouldPass?1u:0u);
  }

  Core::Types::SignedNodesFifo mainQueue_;
  Processor::InterfaceAutoPtr  interface_;
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
  p.process( SignedNode(ti.node_, "me") ); // this call should add 1 element to 'changed'
                                           // elements set, and processor should forward it
                                           // to the main queue.
  // wait for the results
  waitForResponse();
  // check results
  ensure_equals("invalid number of elements", mainQueue_.size(), 1u);
  Core::Types::SignedNode sn=mainQueue_.pop();
  ensure("requested element not found", sn.getNode().get()==ti.node_.get() );
  ensure("invalid sign in entry", sn.getReporterName()==ti.getName() );
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
  p.process( SignedNode(ti.node_,  "me") ); // this call should add 2 elements to 'changed'
  p.process( SignedNode(ti.node2_, "me") ); // elements set, and processor should forward it
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

// test if ECL works - reject test
template<>
template<>
void testObj::test<6>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  ecl.add("myexception");
  checkECL(ecl, "myexception", false);
}

// test if ECL works - accept test
template<>
template<>
void testObj::test<7>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultReject();
  ecl.add("myexception");
  checkECL(ecl, "myexception", true);
}

// test if ECL works - reject self test
template<>
template<>
void testObj::test<8>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  checkECL(ecl, "testinterfacename", false);
}

} // namespace tut