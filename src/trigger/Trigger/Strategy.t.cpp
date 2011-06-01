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
using namespace Core::Types::Proc;
using namespace TestHelpers::Persistency;

namespace
{

struct TestTrigger: public Strategy
{
  TestTrigger(void):
    Strategy( TypeName("testtrigger"), InstanceName("triggername") ),
    callsCriteria_(0),
    callsTrigger_(0),
    criteria_(false),
    node_( makeNewLeaf() )
  {
  }

  virtual bool matchesCriteria(const Persistency::ConstGraphNodePtrNN &n) const
  {
    ++callsCriteria_;
    checkNode(n);
    return criteria_;
  }

  virtual void trigger(const Persistency::ConstGraphNodePtrNN &n)
  {
    ++callsTrigger_;
    checkNode(n);
  }

  void checkNode(const Persistency::ConstGraphNodePtrNN &n) const
  {
    tut::ensure("invalid node", n.get()==node_.get() );
  }

  mutable int    callsCriteria_;
  int            callsTrigger_;
  bool           criteria_;
  GraphNodePtrNN node_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestTrigger::ChangedNodes cn_;
  TestTrigger               tt_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Strategy");
} // unnamed namespace


namespace tut
{

// check if type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid type", tt_.getTriggerType().str(), "testtrigger");
}

// check if process skips trigger() when criteria is not met
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid initial number of calls to matchesCriteria", tt_.callsCriteria_, 0);
  ensure_equals("invalid initial number of calls to trigger", tt_.callsTrigger_, 0);
  // call
  tt_.criteria_=false;
  tt_.process(tt_.node_, cn_);
  // check
  ensure_equals("invalid number of calls to matchesCriteria", tt_.callsCriteria_, 1);
  ensure_equals("invalid number of calls to trigger", tt_.callsTrigger_, 0);
}


namespace
{
struct TestLoopTrigger: public Strategy
{
  TestLoopTrigger(void):
    Strategy( TypeName("testlooptrigger"), InstanceName("triggername") )
  {
  }

  virtual bool matchesCriteria(const Persistency::ConstGraphNodePtrNN &) const
  {
    return true;
  }

  virtual void trigger(const Persistency::ConstGraphNodePtrNN &)
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
  CallableLT(TestLoopTrigger::ChangedNodes *cn,
             TestLoopTrigger               *tlt):
    cn_(cn),
    tlt_(tlt)
  {
  }

  void operator()(void)
  {
    assert( cn_->size()==0 );
    tlt_->process( makeNewLeaf(), *cn_);
    assert( cn_->size()==0 );
  }

  TestLoopTrigger::ChangedNodes *cn_;
  TestLoopTrigger               *tlt_;
}; // struct CollableLT
} // unnmaed namespace

// check if interruptionPoint() works as expected
template<>
template<>
void testObj::test<3>(void)
{
  TestLoopTrigger::ChangedNodes cn;
  TestLoopTrigger               tlt;
  Base::Threads::ThreadJoiner th( (CallableLT(&cn, &tlt)) );
  th->interrupt();
  th->join();
}

// check if process calls trigger() when criteria is met
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid initial number of calls to matchesCriteria", tt_.callsCriteria_, 0);
  ensure_equals("invalid initial number of calls to trigger", tt_.callsTrigger_, 0);
  // call
  tt_.criteria_=true;
  tt_.process(tt_.node_, cn_);
  // check
  ensure_equals("invalid number of calls to matchesCriteria", tt_.callsCriteria_, 1);
  ensure_equals("invalid number of calls to trigger", tt_.callsTrigger_, 1);
}

// check if the same nod is not triggered more than once
template<>
template<>
void testObj::test<5>(void)
{
  tt_.criteria_=true;
  // pre-conditions
  ensure_equals("invalid initial number of calls to matchesCriteria", tt_.callsCriteria_, 0);
  ensure_equals("invalid initial number of calls to trigger", tt_.callsTrigger_, 0);

  tt_.process(tt_.node_, cn_);       // first call
  // sanity check
  ensure_equals("matchesCriteria not called at all", tt_.callsCriteria_, 1);
  ensure_equals("trigger not called at all", tt_.callsTrigger_, 1);

  tt_.process(tt_.node_, cn_);       // second call
  // check - if element was already triggered, it should not be event checked for
  // maching predefined criterias.
  ensure_equals("invalid number of calls to matchesCriteria", tt_.callsCriteria_, 1);
  ensure_equals("invalid number of calls to trigger", tt_.callsTrigger_, 1);
}

// smoke test for sending heartbeat
template<>
template<>
void testObj::test<6>(void)
{
  tt_.heartbeat(42u);
}

} // namespace tut
