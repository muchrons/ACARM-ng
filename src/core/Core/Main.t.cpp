/*
 * Main.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Core/Main.hpp"
#include "TestHelpers/TestBase.hpp"

using namespace Core;
using Base::Threads::ThreadJoiner;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/Main");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Main m;
}

// test stopping request and joining
template<>
template<>
void testObj::test<2>(void)
{
  Main m;
  m.stop();
  m.waitUntilDone();
}

namespace
{
struct SignalSender
{
  void operator()(void)
  {
    while(true)
    {
      ensure_equals("kill() failed", kill( getpid(), SIGINT), 0);
      boost::this_thread::sleep( boost::posix_time::seconds(1) );
    }
  }
}; // struct SignalSender
} // unnamed namespace

// test stopping request from other thread
template<>
template<>
void testObj::test<3>(void)
{
  Main         m;
  ThreadJoiner th( (SignalSender()) );
  m.waitUntilDone();
}

} // namespace tut
