/*
 * ThreadJoiner.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "Base/Threads/ThreadJoiner.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
  TestClass(void):
    state_(0)
  {
  }

  volatile int state_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/ThreadJoiner");
} // unnamed namespace


namespace tut
{

namespace
{

struct TestThread
{
  explicit TestThread(volatile int *state):
    state_(state)
  {
    assert(state_!=NULL);
    tut::ensure_equals("invalid state in thread", *state_, 0);
  }

  ~TestThread(void)
  {
    assert(state_!=NULL);
    *state_=2;
  }

  void operator()(void)
  {
    assert(state_!=NULL);
    *state_=1;
    while(true)
    {
      boost::this_thread::interruption_point();
      usleep(10*1000);
    }
  }

private:
  volatile int *state_;
}; // struct TestThread

} // unnamed namespace


// test default initialization / destruction
template<>
template<>
void testObj::test<1>(void)
{
  ThreadJoiner thj( (TestThread(&state_)) );
}

// test operator ->
template<>
template<>
void testObj::test<2>(void)
{
  ThreadJoiner thj( (TestThread(&state_)) );
  thj->interrupt();
}

// test d-tor after thread has been already joined
template<>
template<>
void testObj::test<3>(void)
{
  ThreadJoiner thj( (TestThread(&state_)) );
  thj->interrupt();
  thj->join();
}

// test if thread is properly stopped
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("invalid initial state", state_, 0);
  // thread run
  {
    ThreadJoiner thj( (TestThread(&state_)) );
    // wait for state flip
    const time_t now=time(NULL);
    while(state_!=1)
    {
      // timeout if nothing happens
      if( time(NULL)>now+5 )
        fail("timeout while waiting for thread (thread not started?)");
      // prevent 100%-time busy-loop
      usleep(11*1000);
    }
    // if state is reached, join thread
  }
  // here thread should be already joined
  ensure_equals("problem with thread joining - race?", state_, 2);
}

} // namespace tut
