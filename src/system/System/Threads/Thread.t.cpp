/*
 * Thread.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/Threads/Thread.hpp"

using namespace System::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Threads/Thread");
} // unnamed namespace


namespace tut
{

namespace
{
struct QuickExit
{
  explicit QuickExit(bool &done):
    done_(&done)
  {
  }

  void operator()(void)
  {
    // exit right away
    *done_=true;
  }

private:
  bool *done_;
}; // struct QuickExit
} // unnamed namespace

// test thread that stops by itself
template<>
template<>
void testObj::test<1>(void)
{
  bool   done=false;
  Thread th( (QuickExit(done)) );
  while(!done)
    boost::this_thread::yield();
  // give it some time
  usleep(250*1000);
}

namespace
{
struct NeverStop
{
  explicit NeverStop(bool &interrupted):
    interrupted_(&interrupted)
  {
    *interrupted_=false;
  }

  void operator()(void)
  {
    try
    {
      while(true)
      {
        boost::this_thread::interruption_point();
        boost::this_thread::yield();
      }
    }
    catch(const boost::thread_interrupted &)
    {
      *interrupted_=true;
    }
  }

private:
  bool *interrupted_;
}; // struct NeverStop
} // unnamed namespace

// test manulal interrupt & join
template<>
template<>
void testObj::test<2>(void)
{
  bool   interrupted=false;
  Thread th( (NeverStop(interrupted)) );
  usleep(100*1000);
  th.interrupt();
  th.join();
  ensure("thread not joined/interrupted", interrupted);
}

// test automatic stopping in d-tor
template<>
template<>
void testObj::test<3>(void)
{
  bool interrupted=false;
  {
    Thread th( (NeverStop(interrupted)) );
    usleep(100*1000);
  }
  ensure("thread not joined/interrupted", interrupted);
}

// test for typedef presence
template<>
template<>
void testObj::test<4>(void)
{
  bool        interrupted=false;
  ThreadPtrNN th( new Thread( (NeverStop(interrupted)) ) );
}


namespace
{
struct GetterID
{
  explicit GetterID(Thread::ID &id):
    id_(&id)
  {
  }

  void operator()(void)
  {
    assert(id_!=NULL);
    *id_=boost::this_thread::get_id();
  }

  Thread::ID *id_;
}; // struct GetterID
} // unnamed namespace

// test getting thread's ID
template<>
template<>
void testObj::test<5>(void)
{
  Thread::ID thID;
  Thread     th( (GetterID(thID)) );
  Thread::ID readID=th.id();
  th.join();
  ensure("invalid id", readID==thID);
}

} // namespace tut
