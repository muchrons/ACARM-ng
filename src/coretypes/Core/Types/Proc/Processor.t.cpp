/*
 * Processor.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "Core/Types/Proc/Processor.hpp"
#include "ConfigConsts/heartbeats.hpp"
#include "ConfigConsts/inputs.hpp"
#include "TestHelpers/TimeoutChecker.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core::Types::Proc;
using namespace Persistency;
using namespace TestHelpers::Persistency;
using Core::Types::SignedNode;
namespace CC=ConfigConsts;

namespace
{

struct TestInterface: public Interface
{
  explicit TestInterface(const EntryControlList &ecl=EntryControlList::createDefaultAccept() ):
    Interface( TypeName("testinterfacetype"), InstanceName("testinterfacename"), ecl),
    calls_(0),
    heartbeats_(0),
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

  virtual void heartbeat(const unsigned int deadline)
  {
    ++heartbeats_;
    tut::ensure_equals("invalid timeout", deadline, CC::maxLostHeartbeats*CC::inputTimeout);
  }

  int  calls_;
  int  heartbeats_;
  Node node_;
  Node node2_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    ti_(new TestInterface),
    interface_(ti_),
    type_("myT"),
    name_("myN")
  {
  }

  void waitForResponse(void)
  {
    // wait for the results up to 2[s]
    const TestHelpers::TimeoutChecker tc(2);
    while( mainQueue_.size()==0 && tc() )
      usleep(10*1000);
    return;
  }

  void checkECL(const EntryControlList &ecl, const std::string &callerTypeStr, const std::string &callerNameStr, bool shouldPass)
  {
    const TypeName     callerType(callerTypeStr);
    const InstanceName callerName(callerNameStr);
    TestInterface *ti=new TestInterface(ecl);
    interface_.reset(ti);
    ProcessorPtrNN p( new Processor(mainQueue_, interface_, ppCfg_) );
    p->process( SignedNode(ti->node_, callerType, callerName) );    // this call should add 1 element to 'changed'
    if(shouldPass)                                                  // should we wait for entry?
      waitForResponse();                                            // wait for other thread
    else
      usleep(150*1000);                                             // do not wait too long if entry is NOT to come
    // check response:
    tut::ensure_equals("invalid collection size", mainQueue_.size(), shouldPass?1u:0u);
  }

  Core::Types::SignedNodesFifo          mainQueue_;
  TestInterface                        *ti_;
  Processor::InterfaceAutoPtr           interface_;
  const TypeName                        type_;
  const InstanceName                    name_;
  const ConfigIO::Preprocessor::Config  ppCfg_;
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
  Processor p(mainQueue_, interface_, ppCfg_);
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
    Processor p(mainQueue_, tmp, ppCfg_);
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
  Processor p(mainQueue_, interface_, ppCfg_);
  p.process( SignedNode(ti_->node_, type_, name_) );    // this call should add 1 element to 'changed'
                                                        // elements set, and processor should forward it
                                                        // to the main queue.
  // wait for the results
  waitForResponse();
  // check results
  ensure_equals("invalid number of elements", mainQueue_.size(), 1u);
  Core::Types::SignedNode sn=mainQueue_.pop();
  ensure("requested element not found", sn.getNode().get()==ti_->node_.get() );
  ensure("invalid sign in entry", sn.getReporterName()==ti_->getName() );
}

// test if multiple calls to process() does not break anything
// (i.e. if 'changed' set is cleared before each call)
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("pre-condition failed", mainQueue_.size(), 0u);
  // process data
  Processor p(mainQueue_, interface_, ppCfg_);
  p.process( SignedNode(ti_->node_,  type_, name_) );   // this call should add 2 elements to 'changed'
  p.process( SignedNode(ti_->node2_, type_, name_) );   // elements set, and processor should forward it
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
  ProcessorPtrNN p( new Processor(mainQueue_, interface_, ppCfg_) );
  ensure("NULL pointer received", p.get()!=NULL );  // could be assert as well
}

// test if ECL works - reject test
template<>
template<>
void testObj::test<6>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  ecl.add( TypeName("myexception") );
  checkECL(ecl, "myexception", "any_name", false);
}

// test if ECL works - accept test
template<>
template<>
void testObj::test<7>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultReject();
  ecl.add( TypeName("myexception") );
  checkECL(ecl, "myexception", "any_name", true);
}

// test if ECL works - reject self test
template<>
template<>
void testObj::test<8>(void)
{
  EntryControlList ecl=EntryControlList::createDefaultAccept();
  checkECL(ecl, "testinterfacetype", "testinterfacename", false);
}

// test heartbeat() call
template<>
template<>
void testObj::test<9>(void)
{
  // test environment
  Processor p(mainQueue_, interface_, ppCfg_);
  ensure_equals("pre-condition failed", ti_->heartbeats_, 0u);
  p.process( SignedNode(ti_->node_, type_, name_) );    // should send heartbeat
  waitForResponse();
  // check
  ensure_equals("heartbeat not sent", ti_->heartbeats_, 1u);
}

// test if heartbeat() calls are not sent constantly
template<>
template<>
void testObj::test<10>(void)
{
  // test environment
  Processor p(mainQueue_, interface_, ppCfg_);
  ensure_equals("pre-condition failed", ti_->heartbeats_, 0u);
  // first action
  p.process( SignedNode(ti_->node_, type_, name_) );    // should send heartbeat
  waitForResponse();
  ensure_equals("heartbeat not sent", ti_->heartbeats_, 1u);
  // second action
  p.process( SignedNode(ti_->node_, type_, name_) );    // should NOT send heartbeat for some time now
  waitForResponse();
  ensure_equals("heartbeat sent again", ti_->heartbeats_, 1u);
}


namespace
{
struct TestInterfaceThrowHeartbeat: public TestInterface
{
  virtual void heartbeat(const unsigned int /*deadline*/)
  {
    throw std::runtime_error("test exception in heartbeat");
  }
};
} // unnamed namespace

// test if exception in heartbeat() does not block processing
template<>
template<>
void testObj::test<11>(void)
{
  // test environment
  ti_=new TestInterfaceThrowHeartbeat();
  interface_.reset(ti_);
  Processor p(mainQueue_, interface_, ppCfg_);
  ensure_equals("pre-condition failed", ti_->calls_, 0u);
  // first action
  p.process( SignedNode(ti_->node_, type_, name_) );    // should discard exception from heartbeat
  waitForResponse();
  ensure_equals("exception in heartbeat stopped processing", ti_->calls_, 1u);
}

} // namespace tut
