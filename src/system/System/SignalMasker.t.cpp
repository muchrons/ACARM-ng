/*
 * SignalMakser.t.cpp
 *
 */
#include <tut.h>
#include <signal.h>

#include "System/SignalMasker.hpp"
#include "System/SignalRegistrator.hpp"

using namespace System;

namespace
{

#define SIG_TO_USE SIGUSR1

volatile int g_counter=0;

void testHandler(int)
{
  ++g_counter;
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
  }

  TestRegistrator tr_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("System/SignalMasker");
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
  SignalMasker m;
  tr_.signal();
  ensure_equals("signals not masked", g_counter, 0u);
}

// test if unblocking works
template<>
template<>
void testObj::test<3>(void)
{
  {
    SignalMasker m;
  }
  tr_.signal();
  ensure_equals("signals were not unblocked", g_counter, 1u);
}

} // namespace tut
