/*
 * Thread.t.cpp
 *
 */
#include <tut.h>

#include "Commons/Threads/Thread.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/TimeoutChecker.hpp"

using namespace std;
using namespace Commons::Threads;

namespace
{

struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Threads/Thread");
} // unnamed namespace


namespace tut
{

namespace
{
struct Callable1
{
  void operator()(void)
  {
  }
}; // sutrct Callable1
} // unnamed namespace

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Thread th( (Callable1()) );
}


namespace
{
struct Callable2
{
  void operator()(void)
  {
    throw std::runtime_error("test");
  }
}; // sutrct Callable2
} // unnamed namespace

// test if exceptions are catched properly
template<>
template<>
void testObj::test<2>(void)
{
  Thread th( (Callable2()) );
}


namespace
{
struct Callable3
{
  Callable3(volatile bool *done):
    done_(done)
  {
    ensure("invalid pre-condition", *done_==false);
  }

  void operator()(void)
  {
    try
    {
      Logger::Node log("test.callable3");
      for(;;)
      {
        LOGMSG_DEBUG(log, "ping :)");
        boost::this_thread::interruption_point();
        boost::this_thread::yield();
      }
    }
    catch(const boost::thread_interrupted &)
    {
      *done_=true;
    }
  }

  volatile bool *done_;
}; // sutrct Callable3
} // unnamed namespace

// test thread joiningin d-tor
template<>
template<>
void testObj::test<3>(void)
{
  volatile bool done=false;
  {
    Thread th( (Callable3(&done)) );
  }

  // wait for response
  const TestHelpers::TimeoutChecker tc(5);
  do
  {
    boost::this_thread::yield();
  }
  while( done!=true && tc() );
  ensure("thread has not been finished", done);
}

// test if THrad is derived from ThreadJoiner
template<>
template<>
void testObj::test<4>(void)
{
  Thread                       th( (Callable2()) );
  Base::Threads::ThreadJoiner &tj=th;
  assert( tj->joinable() );     // anything - just for compiler to stop complaining. :)
}

} // namespace tut
