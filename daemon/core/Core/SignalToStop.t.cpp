/*
 * SignalToStop.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <boost/thread.hpp>
#include <cassert>

#include "Core/SignalToStop.hpp"

using namespace Core;

namespace
{

struct SignalHUP: public SignalToStop
{
  explicit SignalHUP(WorkThreads *wt):
    SignalToStop(SIGHUP, wt)
  {
  }
}; // struct SignalHUP

struct TestClass
{
  TestClass(void)
  {
  }

  WorkThreads wt_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/SignalToStop");
} // unnamed namespace


namespace tut
{

// test reaction on given signal
template<>
template<>
void testObj::test<1>(void)
{
  SignalHUP sig(&wt_);
  // send proper signal
  const int ret=kill( getpid(), SIGHUP );
  ensure_equals("sending signal failed", ret, 0);
  // if it failed, this call will block
  wt_.waitUntilDone();
}

// test reaction when NULL pointer given
template<>
template<>
void testObj::test<2>(void)
{
  SignalHUP sig(NULL);
  // send proper signal
  const int ret=kill( getpid(), SIGHUP );
  ensure_equals("sending signal failed", ret, 0);
  // smoke test - nothing should happen
}

} // namespace tut
