/*
 * InterfaceImpl.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core::Types::Proc;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestStrategyParams
{
  TestStrategyParams(void):
    calls_(0),
    heartbeats_(0)
  {
  }

  mutable int calls_;
  mutable int heartbeats_;
}; // struct TestStrategyParams

struct TestStrategy
{
  TestStrategy(const InstanceName &/*name*/, const TestStrategyParams &p):
    p_(p)
  {
    p_.calls_=0;
  }

  void process(GraphNodePtrNN, Interface::ChangedNodes&)
  {
    ++p_.calls_;
  }

  void heartbeat(unsigned int)
  {
    ++p_.heartbeats_;
  }

  static EntryControlList createEntryControlList(void)
  {
    return EntryControlList::createDefaultAccept();
  }

  const TestStrategyParams &p_;
};


namespace
{
int testStrategyNoParmCalls;
int testStrategyNoParmHeartbeats;
} // unnamed namespace

struct TestStrategyNoParm
{
  TestStrategyNoParm(void)
  {
    testStrategyNoParmCalls     =0;
    testStrategyNoParmHeartbeats=0;
  }

  void process(GraphNodePtrNN, Interface::ChangedNodes&)
  {
    ++testStrategyNoParmCalls;
  }

  void heartbeat(unsigned int)
  {
    ++testStrategyNoParmHeartbeats;
  }

  static EntryControlList createEntryControlList(void)
  {
    return EntryControlList::createDefaultAccept();
  }
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    impl_( TypeName("sometype"), InstanceName("somename"), params_)
  {
  }

  TestStrategyParams                              params_;
  InterfaceImpl<TestStrategy, TestStrategyParams> impl_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/InterfaceImpl");
} // unnamed namespace


namespace tut
{

// test getting type
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type set", impl_.getType().str(), "sometype");
}
// test getting name
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid name set", impl_.getName().str(), "somename");
}

// test passing call to process
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("pre-condition failed", params_.calls_, 0);
  Interface::ChangedNodes changed;
  impl_.process( makeNewLeaf(), changed );
  ensure_equals("process() not forwarded", params_.calls_, 1);
}

// test test 2-arg c-tor (should compile)
template<>
template<>
void testObj::test<4>(void)
{
  InterfaceImpl<TestStrategyNoParm> tmp( TypeName("sometype") , InstanceName("somename") );
}

// test process() on non-param strategy object
template<>
template<>
void testObj::test<5>(void)
{
  InterfaceImpl<TestStrategyNoParm> tmp( TypeName("sometype") , InstanceName("somename") );
  ensure_equals("pre-condition failed", testStrategyNoParmCalls, 0);
  Interface::ChangedNodes changed;
  tmp.process( makeNewLeaf(), changed );
  ensure_equals("process() not forwarded", testStrategyNoParmCalls, 1);
}

// test passing heartbeats to strategy
template<>
template<>
void testObj::test<6>(void)
{
  ensure_equals("pre-condition failed", params_.heartbeats_, 0);
  impl_.heartbeat(42u);
  ensure_equals("call not passed", params_.heartbeats_, 1);
}

// test heartbeats on non-param strategy object
template<>
template<>
void testObj::test<7>(void)
{
  InterfaceImpl<TestStrategyNoParm> tmp( TypeName("sometype") , InstanceName("somename") );
  ensure_equals("pre-condition failed", testStrategyNoParmHeartbeats, 0);
  tmp.heartbeat(42u);
  ensure_equals("process() is not virtual", testStrategyNoParmHeartbeats, 1);
}

} // namespace tut
