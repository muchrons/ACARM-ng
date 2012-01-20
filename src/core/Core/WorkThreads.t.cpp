/*
 * main.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <boost/thread.hpp>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Core/WorkThreads.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/TimeoutChecker.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
  Core::Types::SignedNodesFifo queue_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/WorkThreads");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  WorkThreads m(queue_);
}

// test stopping request and joining
template<>
template<>
void testObj::test<2>(void)
{
  WorkThreads m(queue_);
  m.stop();
  m.waitUntilDone();
}


namespace
{
struct Stopper
{
  Stopper(volatile bool *ready, WorkThreads *m):
    ready_(ready),
    m_(m)
  {
  }

  void operator()(void)
  {
    const TestHelpers::TimeoutChecker tc(5);
    assert(ready_!=NULL);
    while( !*ready_ && tc() )
      usleep(30*1000);
    usleep(50*1000);
    assert(m_!=NULL);
    m_->stop();
  }

  volatile bool *ready_;
  WorkThreads          *m_;
}; // struct Stopper
} // unnamed namespace

// test interruption form other thread.
template<>
template<>
void testObj::test<3>(void)
{
  volatile bool               ready=false;
  WorkThreads                 m(queue_);
  Base::Threads::ThreadJoiner th( Stopper(&ready, &m) );
  ready=true;
  m.waitUntilDone();
}

// test for bug - when exception is thrown when second
// threads poll is being created, then d-tor is NOT called
// and conditional is being destroyed in main thread, though
// other thread is already running. to enforce this situation special
// config file has been provided.
template<>
template<>
void testObj::test<4>(void)
{
  readConfigFile("testdata/two_threads_exception_bug.xml");
  try
  {
    WorkThreads m(queue_);  // test's core - this must not SEGV!
    fail("WorkThreads' c-tor didn't throw on invalid configuration");
  }
  catch(const Commons::Exception &)
  {
    // this is expected
  }
}

} // namespace tut
