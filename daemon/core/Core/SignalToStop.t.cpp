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

using namespace boost;
using namespace Core;

namespace
{

struct SignalHUP: public SignalToStop
{
  explicit SignalHUP(Main &main):
    SignalToStop(SIGHUP, main)
  {
  }
}; // struct SignalHUP

struct TestClass
{
  TestClass(void)
  {
  }

  Main main_;
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
  SignalHUP sig(main_);
  // send proper signal
  const int ret=kill( getpid(), SIGHUP );
  ensure_equals("sending signal failed", ret, 0);
  // if it failed, this call will block
  main_.waitUntilDone();
}

} // namespace tut
