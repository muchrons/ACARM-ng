/*
 * WaitingLockData.t.cpp
 *
 */
#include <tut.h>
#include <memory>
#include <boost/thread/thread.hpp>
#include <cassert>

#include "Base/Threads/WriteLock.hpp"
#include "Persistency/detail/NonCyclicAdder/WaitingLockData.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace boost;
using namespace Base::Threads;
using namespace Persistency;
using namespace Persistency::detail;

namespace
{

struct TestClass
{
  TestClass(void):
    ma1_( new MetaAlert( makeNewAlert(), 111u ) ),
    ma2_( new MetaAlert( makeNewAlert(), 111u ) ),
    conn_(new TestIOConnection),
    t_( conn_->createNewTransaction("gn_test") ),
    leaf_( makeLeaf() )
  {
  }

  GraphNodePtrNN makeLeaf(void)
  {
    return GraphNodePtrNN( new GraphNode( makeNewAlert(), 111u, conn_, t_) );
  }

  MetaAlertPtrNN      ma1_;
  MetaAlertPtrNN      ma2_;
  IO::ConnectionPtrNN conn_;
  IO::Transaction     t_;

  GraphNodePtrNN      leaf_;

  ReadWriteMutex      mutexRW_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/detail/NonCyclicAdder/WaitingLockData");
} // unnamed namespace


namespace tut
{

// try c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  WaitingLockData wld;
  ensure("default pointer's not NULL", wld.getPtr().get()==NULL );
}

// test setting poitner
template<>
template<>
void testObj::test<2>(void)
{
  WaitingLockData wld;
  wld.setPtr( leaf_.shared_ptr() );
  ensure("invalid pointer", leaf_==wld.getPtr() );
}

// test imediate return when pointer changed
template<>
template<>
void testObj::test<3>(void)
{
  WriteLock       lock(mutexRW_);
  WaitingLockData wld;
  GraphNodePtrNN  leaf2=makeLeaf();
  wld.setPtr( leaf_.shared_ptr() );
  ReadTryLock     rtl(mutexRW_);
  ensure("locking for read only didn't failed", rtl.ownsLock()==false );
  ensure("invalid pointer", wld.getWhenDifferOrLocked( makeLeaf().shared_ptr(), rtl)==leaf_ );
}

// test getting when lock can be obtained
template<>
template<>
void testObj::test<4>(void)
{
  WaitingLockData    wld;
  ReadTryLock        rtl(mutexRW_);
  wld.setPtr( leaf_.shared_ptr() );
  ensure("locking for read only failed", rtl.ownsLock()==true );
  ensure("invalid pointer", wld.getWhenDifferOrLocked( leaf_.shared_ptr(), rtl)==leaf_ );
}


namespace
{
struct TestThread
{
  TestThread(WaitingLockData *wld, volatile int *data, ReadWriteMutex *mutexRW):
    wld_(wld),
    data_(data),
    mutexRW_(mutexRW)
  {
    assert(wld_    !=NULL);
    assert(data_   !=NULL);
    assert(mutexRW_!=NULL);
    *data_=0;
  }

  void operator()(void)
  {
    *data_=1;
    ReadTryLock rtl(*mutexRW_);
    wld_->getWhenDifferOrLocked( GraphNodePtr(), rtl );
    *data_=2;
  }

  WaitingLockData *wld_;
  volatile int    *data_;
  ReadWriteMutex  *mutexRW_;
}; // struct TestThread
} // unnamed namespace

// test if setting new pointer value releases lock
template<>
template<>
void testObj::test<5>(void)
{
  WaitingLockData     wld;
  volatile int        state=0;
  auto_ptr<WriteLock> wlock( new WriteLock(mutexRW_) );
  assert( wlock.get()!=NULL );
  TestThread          tt(&wld, &state, &mutexRW_);
  thread              th(tt);
  // wait for thread a little
  while(state!=1)
    boost::thread::yield();
  // release lock by setting new value
  wld.setPtr( leaf_.shared_ptr() );
  // join - if call exited, thread will join cleanly
  th.join();
}

// test signalAll() call
template<>
template<>
void testObj::test<6>(void)
{
  WaitingLockData     wld;
  volatile int        state=0;
  auto_ptr<WriteLock> wlock( new WriteLock(mutexRW_) );
  assert( wlock.get()!=NULL );
  TestThread          tt(&wld, &state, &mutexRW_);
  thread              th(tt);
  // wait for thread a little
  while(state!=1)
    boost::thread::yield();
  // release lock by releaseing write-lock
  wlock.reset();
  wld.signalAll();
  // join - if call exited, thread will join cleanly
  th.join();
}

} // namespace tut
