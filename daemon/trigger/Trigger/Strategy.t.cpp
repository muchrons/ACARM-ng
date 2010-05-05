/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Trigger/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Trigger;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestTrigger: public Strategy
{
  TestTrigger(void):
    Strategy("testtrigger"),
    callsCriteria_(0),
    callsTrigger_(0),
    criteria_(false),
    node_( makeNewLeaf() )
  {
  }

  virtual bool matchesCriteria(const NodeType &n)
  {
    ++callsCriteria_;
    checkNode(n);
    return criteria_;
  }

  virtual void trigger(const NodeType &n)
  {
    ++callsTrigger_;
    checkNode(n);
  }

  void checkNode(const NodeType &n) const
  {
    tut::ensure("invalid node", &n==node_.get() );
  }

  int            callsCriteria_;
  int            callsTrigger_;
  bool           criteria_;
  GraphNodePtrNN node_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestTrigger tt_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Strategy");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", tt_.getTriggerName(), "testtrigger");
}

// check if process skips trigger() when criteria is not met
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid initial number of calls to matchesCriteria",
                tt_.callsCriteria_, 0);
  ensure_equals("invalid initial number of calls to trigger",
                tt_.callsTrigger_, 0);
  // call
  tt_.criteria_=false;
  tt_.process(tt_.node_);
  // check
  ensure_equals("invalid number of calls to matchesCriteria",
                tt_.callsCriteria_, 1);
  ensure_equals("invalid number of calls to trigger",
                tt_.callsTrigger_, 0);
}


namespace
{
struct TestLoopTrigger: public Strategy
{
  TestLoopTrigger(void):
    Strategy("testlooptrigger")
  {
  }

  virtual bool matchesCriteria(const NodeType&)
  {
    return true;
  }

  virtual void trigger(const NodeType&)
  {
    for(;;)
    {
      boost::this_thread::yield();  // avoid too much CPU waste
      interruptionPoint();
    }
  }
}; // struct TestLoopTrigger

struct CallableLT
{
  void operator()(void)
  {
    tlt_.process( makeNewLeaf() );
  }

  TestLoopTrigger tlt_;
}; // struct CollableLT
} // unnmaed namespace

// check if interruptionPoint() works as expected
template<>
template<>
void testObj::test<3>(void)
{
  CallableLT clt;
  Base::Threads::ThreadJoiner th( boost::ref(clt) );
  th->interrupt();
  th->join();
}

// check if process calls trigger() when criteria is met
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid initial number of calls to matchesCriteria",
                tt_.callsCriteria_, 0);
  ensure_equals("invalid initial number of calls to trigger",
                tt_.callsTrigger_, 0);
  // call
  tt_.criteria_=true;
  tt_.process(tt_.node_);
  // check
  ensure_equals("invalid number of calls to matchesCriteria",
                tt_.callsCriteria_, 1);
  ensure_equals("invalid number of calls to trigger",
                tt_.callsTrigger_, 1);
}

} // namespace tut
