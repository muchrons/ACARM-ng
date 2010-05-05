/*
 * StrategyBase.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread.hpp>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Filter/StrategyBase.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter;
using namespace Persistency;

namespace
{

struct TestFilter: public StrategyBase
{
  TestFilter(void):
    StrategyBase("testfilter")
  {
  }

  void testLog(void) const
  {
  LOGMSG_DEBUG(log_, "StrategyBase logging test");
  }

  void testIP(void) const
  {
    interruptionPoint();
  }

  void testConnection(void) const
  {
    tut::ensure("connection is inactive", conn_.get()!=NULL );
  }
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestFilter tf_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/StrategyBase");
} // unnamed namespace


namespace tut
{

// check if name has been saved correctly
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name", tf_.getFilterName(), "testfilter");
}

// test logging (smoke test)
template<>
template<>
void testObj::test<2>(void)
{
  tf_.testLog();
}


namespace
{
struct CallableLF
{
  void operator()(void)
  {
    TestFilter tf;
    // loop until break is required
    for(;;)
      tf.testIP();
  }
}; // struct CollableLF
} // unnmaed namespace

// check if interruptionPoint() works as expected
template<>
template<>
void testObj::test<3>(void)
{
  CallableLF clf;
  Base::Threads::ThreadJoiner th(clf);
  th->interrupt();
  th->join();
}

// test persistency connection
template<>
template<>
void testObj::test<4>(void)
{
  tf_.testConnection();
}

} // namespace tut
