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
  void operator()(void)
  {
    while(true)
    {
      boost::this_thread::interruption_point();
      usleep(10*1000);
    }
  }
}; // struct TestThread

} // unnamed namespace


// test default initialization / destruction
template<>
template<>
void testObj::test<1>(void)
{
  ThreadJoiner thj( TestThread() );
}

// test operator ->
template<>
template<>
void testObj::test<2>(void)
{
  ThreadJoiner thj( (TestThread()) );
  thj->interrupt();
}

// test d-tor after thread has been already joined
template<>
template<>
void testObj::test<3>(void)
{
  ThreadJoiner thj( (TestThread()) );
  thj->interrupt();
  thj->join();
}

} // namespace tut
