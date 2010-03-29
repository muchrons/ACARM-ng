/*
 * SignalRegistrator.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <cstring>
#include <cassert>

#include "System/SignalRegistrator.hpp"

using namespace System;

namespace
{

#define SIG_TO_USE SIGUSR1

int counter=0;
void testHandler(int)
{
  ++counter;
}

struct TestRegistrator: public SignalRegistrator
{
  TestRegistrator(int signum=SIG_TO_USE, void (*handler)(int)=testHandler):
    SignalRegistrator(signum, handler)
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
    counter=0;

    // set default behaviour for used signal to 'ignore'.
    struct sigaction act;
    memset(&act, 0, sizeof(act) );
    act.sa_handler=SIG_IGN;
    const int ret=sigaction(SIG_TO_USE, &act, &old_);
    assert(ret==0 && "registration of signal failed");
  }

  ~TestClass(void)
  {
    // return oryginal handler
    const int ret=sigaction(SIG_TO_USE, &old_, NULL);
    assert(ret==0 && "deregistration of signal failed");
  }

  struct sigaction old_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("System/SignalRegistrator");
} // unnamed namespace


namespace tut
{

// test if registration and deregistration does not do anything
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("counter not reseted", counter, 0);
  TestRegistrator tr;
  ensure_equals("counter changed", counter, 0);
}

// test throwing on invalid signal
template<>
template<>
void testObj::test<2>(void)
{
  try
  {
    TestRegistrator tr(-12345); // should throw
    fail("SignalRegistrator didn't throw on invalid signal");
  }
  catch(const System::Exception &)
  {
    // this is expected
  }
}

// test throwing on invalid handler
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    TestRegistrator tr(SIGUSR1, NULL);  // should throw
    fail("SignalRegistrator didn't throw on invalid handler");
  }
  catch(const System::Exception &)
  {
    // this is expected
  }
}

// check if registration works
template<>
template<>
void testObj::test<4>(void)
{
  TestRegistrator tr;
  ensure_equals("precondition failed", counter, 0);
  tr.signal();
  ensure_equals("signal didn't worked", counter, 1);
}

// check if deregistration works
template<>
template<>
void testObj::test<5>(void)
{
  {
    TestRegistrator tr;
  } // deregistration goes here
  ensure_equals("precondition failed", counter, 0);
  TestRegistrator::signal();
  ensure_equals("deregistration didn't worked", counter, 0);
}

} // namespace tut
