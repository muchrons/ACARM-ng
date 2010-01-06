/*
 * Processor.t.cpp
 *
 */
#include <tut.h>

#include "Trigger/Processor.hpp"
#include "Persistency/Stubs/TestHelpers.hpp"

using namespace Trigger;
using namespace Persistency;
using namespace Persistency::Stubs;

namespace
{

struct TestTrigger: public Interface
{
  TestTrigger(void):
    Interface("testtrigger"),
    callsCriteria_(0),
    callsTrigger_(0)
  {
  }

  virtual bool matchCriteria(const GraphNode&)
  {
    ++callsCriteria_;
    return true;
  }

  virtual void trigger(const GraphNode&)
  {
    ++callsTrigger_;
  }

  int callsCriteria_;
  int callsTrigger_;
};


struct TestClass
{
  TestClass(void):
    trigger_(new TestTrigger)
  {
  }

  Core::Types::NodesFifo      mainQueue_;
  Processor::InterfaceAutoPtr trigger_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Processor");
} // unnamed namespace


namespace tut
{

// test d-tor when nothing has been called (thread should be aborted)
template<>
template<>
void testObj::test<1>(void)
{
  Processor p(trigger_);
  usleep(30*1000);                      // wait a while to ensure thread is running
  // when exiting this should not block
}

// test c-tor throw when NULL trigger passed
template<>
template<>
void testObj::test<2>(void)
{
  Processor::InterfaceAutoPtr tmp;
  ensure("pre-condition failed", tmp.get()==NULL );
  try
  {
    Processor p(tmp);
    fail("processor's c-tor didn't throw on NULL trigger");
  }
  catch(const ExceptionInvalidTrigger&)
  {
    // this is expected
  }
}

// test whole call sequence
template<>
template<>
void testObj::test<3>(void)
{
  TestTrigger &t=dynamic_cast<TestTrigger&>(*trigger_);
  ensure_equals("pre-condition 1 failed", t.callsCriteria_, 0);
  ensure_equals("pre-condition 2 failed", t.callsTrigger_,  0);
  // process data
  Processor                   p(trigger_);
  Persistency::GraphNodePtrNN node( makeNewLeaf() );
  p.process(node);              // this call should pass both processing user-calls
  // wait for the results
  for(int i=0; i<10; ++i)
    if( t.callsTrigger_>0 )
      break;
    else
      usleep(50*1000);
  // check results
  ensure_equals("matchCriteria not called", t.callsCriteria_, 1);
  ensure_equals("trigger not called",       t.callsTrigger_,  1);
}

} // namespace tut
