/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <boost/shared_ptr.hpp>

#include "Commons/Threads/Thread.hpp"
#include "PythonAPI/ExceptionFromScript.hpp"
#include "Trigger/Python/Strategy.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Commons::Threads;
using namespace Persistency;
using namespace Trigger::Python;
using namespace TestHelpers::Persistency;
using namespace Core::Types::Proc;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  Config mkConfig(const Config::Path &script="testdata/do_nothing.py") const
  {
    const Trigger::Simple::ThresholdConfig th("1", "2");
    return Config(th, script);
  }

  Trigger::Strategy::ChangedNodes cn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Python/Strategy");
} // unnamed namespace


namespace tut
{

// check if trigger type has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  Strategy s(InstanceName("mysnaketrigger"), mkConfig());
  ensure_equals("invalid trigger type", s.getTriggerType().str(), "python");
}

// test forwarding call to python code
template<>
template<>
void testObj::test<2>(void)
{
  Strategy s(InstanceName("mysnaketrigger"), mkConfig());
  s.process(makeNewTree1(), cn_);
}

// check if exception is thrown when script does not exist
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    Strategy s(InstanceName("test"), mkConfig("/script/does/not/exist.py"));
    fail("exception not thrown");
  }
  catch(const Commons::Exception&)
  {
    // this is expected
  }
}

// check if exception is thrown when script does not set required 'derived' variable
template<>
template<>
void testObj::test<4>(void)
{
  try
  {
    Strategy s(InstanceName("test"), mkConfig("testdata/null_object.py"));
    fail("exception not thrown on NULL");
  }
  catch(const Commons::Exception&)
  {
    // this is expected
  }
}

// test forwarding exception from scripts
template<>
template<>
void testObj::test<5>(void)
{
  Strategy s(InstanceName("test"), mkConfig("testdata/raise_exception.py"));
  try
  {
    s.process(makeNewTree1(), cn_);
    fail("exception not re-thrown from the script");
  }
  catch(const PythonAPI::ExceptionFromScript&)
  {
    // this is expected
  }
}


namespace
{
struct LoopThread
{
  LoopThread(const size_t loops, const Config &cfg):
    loops_(loops),
    cfg_(cfg)
  {
  }

  void operator()(void) const
  {
    const GraphNodePtrNN            node=makeNewTree1();
    Trigger::Strategy::ChangedNodes cn;
    Strategy                        s(InstanceName("loopthread"), cfg_);
    for(size_t i=0; i<loops_; ++i)
    {
      cn.clear();
      s.process(node, cn);
    }
  }

private:
  size_t loops_;
  Config cfg_;
}; // struct LoopThread
} // unnamed namespace

// test running multiple strategies from different threads (they must execute sequentially)
template<>
template<>
void testObj::test<6>(void)
{
  // config
  typedef boost::shared_ptr<Thread> ThreadPtr;
  std::vector<ThreadPtr>  ths;
  const size_t            count=30;
  const size_t            loops=1000;
  const Config            cfg=mkConfig("testdata/counter_trigger.py");
  // init and run
  for(size_t i=0; i<count; ++i)
  {
    LoopThread lt(loops, cfg);
    ThreadPtr  th(new Thread(lt));
    ths.push_back(th);
  }

  // join
  for(std::vector<ThreadPtr>::const_iterator it=ths.begin(); it!=ths.end(); ++it)
  {
    assert( (*it).get()!=NULL );
    (*(*it).get())->join();
  }
}

} // namespace tut
