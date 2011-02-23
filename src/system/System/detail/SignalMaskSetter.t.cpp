/*
 * SignalMaskSetter.t.cpp
 *
 */
#include <tut.h>
#include <signal.h>
#include <boost/thread.hpp>

#include "System/detail/SignalMaskSetter.hpp"
#include "System/SignalRegistrator.hpp"
#include "System/Threads/Thread.hpp"
#include "System/Threads/Sync.hpp"

using namespace System;

namespace
{

#define SIG_TO_USE SIGUSR1

volatile int      g_counter=0;
boost::thread::id g_id;

void testHandler(int)
{
  ++g_counter;
  g_id=boost::this_thread::get_id();
}

struct TestRegistrator: public SignalRegistrator
{
  TestRegistrator(void):
    SignalRegistrator(SIG_TO_USE, testHandler)
  {
  }

  static void signal(void)
  {
    const int ret=kill( getpid(), SIG_TO_USE);
    tut::ensure_equals("kill() failed", ret, 0);
  }
};


struct TestClass
{
  TestClass(void)
  {
    // reset counter
    g_counter=0;
    // set signals set
    sigemptyset(&set_);
    sigaddset(&set_, SIG_TO_USE);
  }

  TestRegistrator tr_;
  sigset_t        set_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("System/detail/SignalMaskSetter");
} // unnamed namespace


namespace tut
{

// self-test: check if signal does work fine
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invailid initial value", g_counter, 0u);
  tr_.signal();
  ensure_equals("self-test failed - OTHER TESTS WON'T TEST ANYTHING", g_counter, 1u);
}

// test if masking singal works
template<>
template<>
void testObj::test<2>(void)
{
  detail::SignalMaskSetter m(set_);
  tr_.signal();
  ensure_equals("signals not masked", g_counter, 0u);
}

// test if unblocking works
template<>
template<>
void testObj::test<3>(void)
{
  {
    detail::SignalMaskSetter m(set_);
  }
  tr_.signal();
  ensure_equals("signals were not unblocked", g_counter, 1u);
}


namespace
{
struct BlockingThread
{
  void operator()(void)
  {
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIG_TO_USE);
    detail::SignalMaskSetter m(set);
    for(time_t to=time(NULL)+5; to>time(NULL);)
    {
      boost::this_thread::interruption_point();
      boost::this_thread::yield();
    }
  }
}; // struct BlockingThread

struct NonBlockingThread
{
  explicit NonBlockingThread(Threads::Sync &s):
    s_(&s)
  {
  }

  void operator()(void)
  {
    assert(s_!=NULL);
    s_->start();
    for(time_t to=time(NULL)+5; to>time(NULL) && g_counter==0;)
    {
      boost::this_thread::interruption_point();
      boost::this_thread::yield();
    }
  }

  Threads::Sync *s_;
}; // struct NonBlockingThread
} // unnamed namespace

// test multi-thread - delivering signals to specific thread
template<>
template<>
void testObj::test<4>(void)
{
  Threads::Sync        sync;
  Threads::Thread      th0( (NonBlockingThread(sync)) );
  Threads::Thread      th1( (BlockingThread()) );
  Threads::Thread      th2( (BlockingThread()) );
  Threads::Thread      th3( (BlockingThread()) );
  Threads::Thread      th4( (BlockingThread()) );
  Threads::Thread      th5( (BlockingThread()) );
  Threads::Thread      th6( (BlockingThread()) );
  detail::SignalMaskSetter m(set_);
  sync.waitForStart();
  const Threads::Thread::ID expected=th0.id();
  ensure("internal test error", Threads::Thread::ID()!=expected);
  // test
  tr_.signal();
  // wait for thread to finish
  th0.join();
  ensure_equals("invalid number of signals", g_counter, 1);
  ensure("invalid thread received signal", g_id==expected );
}

} // namespace tut
