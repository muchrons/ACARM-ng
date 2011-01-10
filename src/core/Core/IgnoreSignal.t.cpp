/*
 * IgnoreSignal.t.cpp
 *
 */
#include <tut.h>
#include <sys/types.h>
#include <signal.h>
#include <cassert>

#include "Core/IgnoreSignal.hpp"

using namespace Core;

namespace
{

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/IgnoreSignal");
} // unnamed namespace


namespace tut
{

// test reaction on given signal
template<>
template<>
void testObj::test<1>(void)
{
  IgnoreSignal sig(SIGPIPE);
  // send proper signal
  const int ret=kill( getpid(), SIGPIPE );
  // if we're here, signal has been ignored properly
  ensure_equals("sending signal failed", ret, 0);
}

} // namespace tut
