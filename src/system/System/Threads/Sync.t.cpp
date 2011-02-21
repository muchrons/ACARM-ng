/*
 * Sync.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "System/Threads/Sync.hpp"
#include "System/Threads/Thread.hpp"

using namespace System::Threads;

namespace
{
struct TestClass
{
  Sync s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Threads/Sync");
} // unnamed namespace


namespace tut
{

namespace
{
struct SyncThread
{
  explicit SyncThread(Sync &s):
    s_(&s)
  {
  }

  void operator()(void)
  {
    s_->waitForStart();
    usleep(100*1000);
    s_->done();
  }

private:
  Sync *s_;
}; // struct SyncThread
} // unnamed namespace

// test thread synchronization
template<>
template<>
void testObj::test<1>(void)
{
  Thread th( (SyncThread(s_)) );
  usleep(100*1000);
  s_.start();
  s_.waitForDone();
}

// test pointer typedef presence
template<>
template<>
void testObj::test<2>(void)
{
  SyncPtrNN s(new Sync);
}

// test if multiple calls to waitForDone() does not break anything
template<>
template<>
void testObj::test<3>(void)
{
  Thread th( (SyncThread(s_)) );
  usleep(100*1000);
  s_.start();
  for(int i=0; i<2; ++i)
    s_.waitForDone();
}
// test if multiple calls to waitForDone() does not break anything for new object
template<>
template<>
void testObj::test<4>(void)
{
  for(int i=0; i<2; ++i)
    s_.waitForDone();
}

} // namespace tut
