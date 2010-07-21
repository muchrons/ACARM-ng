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
  mutable int calls_;
}; // struct TestStrategyParams

struct TestStrategy
{
  explicit TestStrategy(const TestStrategyParams &p):
    p_(p)
  {
    p_.calls_=0;
  }

  void process(GraphNodePtrNN, Interface::ChangedNodes&)
  {
    ++p_.calls_;
  }

  static EntryControlList createEntryControlList(void)
  {
    return EntryControlList::createDefaultAccept();
  }

  const TestStrategyParams &p_;
};


namespace
{
int testStrategyNoParmCalls=0;
} // unnamed namespace

struct TestStrategyNoParm
{
  TestStrategyNoParm(void)
  {
    testStrategyNoParmCalls=0;
  }

  void process(GraphNodePtrNN, Interface::ChangedNodes&)
  {
    ++testStrategyNoParmCalls;
  }

  static EntryControlList createEntryControlList(void)
  {
    return EntryControlList::createDefaultAccept();
  }
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    impl_("sometype", "somename", params_)
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
  ensure_equals("invalid type set", impl_.getType(), "sometype");
}
// test getting name
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid name set", impl_.getName(), "somename");
}

// test passing call to process
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("pre-condition failed", params_.calls_, 0);
  Interface::ChangedNodes changed;
  impl_.process( makeNewLeaf(), changed );
  ensure_equals("process() is not virtual", params_.calls_, 1);
}

// test test 2-arg c-tor (should compile)
template<>
template<>
void testObj::test<4>(void)
{
  InterfaceImpl<TestStrategyNoParm> tmp("sometype", "somename");
}

// test process() on non-param strategy object
template<>
template<>
void testObj::test<5>(void)
{
  InterfaceImpl<TestStrategyNoParm> tmp("sometype", "somename");
  ensure_equals("pre-condition failed", testStrategyNoParmCalls, 0);
  Interface::ChangedNodes changed;
  tmp.process( makeNewLeaf(), changed );
  ensure_equals("process() is not virtual", testStrategyNoParmCalls, 1);
}

} // namespace tut
