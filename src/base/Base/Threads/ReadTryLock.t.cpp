/*
 * ReadTryLock.t.cpp
 *
 */
#include <tut.h>
#include <memory>
#include <boost/thread/thread.hpp>

#include "Base/Threads/WriteLock.hpp"
#include "Base/Threads/ReadTryLock.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace std;
using namespace Base::Threads;

namespace
{
struct TestClass
{
  ReadWriteMutex m_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/ReadTryLock");
} // unnamed namespace


namespace tut
{

// test locking
template<>
template<>
void testObj::test<1>(void)
{
  ReadTryLock lock(m_);
  ensure("mutex not locked", lock.ownsLock() );
}

// re-locking
template<>
template<>
void testObj::test<2>(void)
{
  {
    ReadTryLock lock(m_);
  }
  // here mutex should be already released
  {
    ReadTryLock lock(m_);
  }
}

// test if read locking works (if read lock does not work, this test will hang)
template<>
template<>
void testObj::test<3>(void)
{
  ReadTryLock lock1(m_);
  ensure("ubale to lock mutex by reader 1", lock1.ownsLock() );
  ReadTryLock lock2(m_);
  ensure("ubale to lock mutex by reader 2", lock2.ownsLock() );
}

namespace
{
struct TestWriter
{
  TestWriter(ReadWriteMutex *m, double *v):
    m_(m),
    v_(v)
  {
    assert(m_!=NULL);
    assert(v_!=NULL);
  }

  void operator()(void)
  {
    WriteLock lock(*m_);
    *v_=42;
  }

private:
  ReadWriteMutex *m_;
  double         *v_;
}; // struct TestWriter
} // unnamed namespace

// test if reading unlocks and so writer may proceed
template<>
template<>
void testObj::test<4>(void)
{
  double                      v=666;
  auto_ptr<ReadTryLock> lock( new ReadTryLock(m_) );
  ensure("ubale to lock mutex by reader", lock->ownsLock() );
  TestWriter                  tw(&m_, &v);
  Base::Threads::ThreadJoiner th(tw);   // run thread
  boost::thread::yield();       // give it some time
  ensure_equals("value changed - lock does not work", v, 666);
  lock.reset();                 // release lock
  th->join();                   // wait until thread's done
  ensure_equals("value not changed", v, 42);
}

// test locking when lock is busy
template<>
template<>
void testObj::test<5>(void)
{
  auto_ptr<WriteLock> lock( new WriteLock(m_) );
  ReadTryLock         tlock(m_);
  ensure("lock obtained, though lock's busy", !tlock.ownsLock() );
  ensure("explicit locking on busy lock didn't failed", !tlock.tryLock() );
  lock.reset();                 // release write lock
  ensure("lock somehow obtained", !tlock.ownsLock() );
  ensure("unable to tryLock() free lock", tlock.tryLock() );
  ensure("locked, but lock now owned", tlock.ownsLock() );
}

// test lock release
template<>
template<>
void testObj::test<6>(void)
{
  {
    ReadTryLock lock(m_);
  }
  // here mutex should be already released
  {
    WriteLock lock(m_);
  }
}

// test timed locking when lock is free
template<>
template<>
void testObj::test<7>(void)
{
  auto_ptr<WriteLock> lock( new WriteLock(m_) );
  ReadTryLock tlock(m_);
  lock.reset();
  ensure("lock obtained, though lock's busy", !tlock.ownsLock() );
  ensure("lock not obtained (during call)", tlock.tryLock(1) );
  ensure("lock not obtained", tlock.ownsLock() );
}

// test timed locking when lock is busy
template<>
template<>
void testObj::test<8>(void)
{
  auto_ptr<WriteLock> lock( new WriteLock(m_) );
  ReadTryLock         tlock(m_);
  ensure("lock obtained, though lock's busy", !tlock.ownsLock() );
  ensure("lock not obtained", !tlock.tryLock(1) );
  ensure("lock obtained, though lock's busy", !tlock.ownsLock() );
}

// test timed locking when lock is free and timeout is 0
template<>
template<>
void testObj::test<9>(void)
{
  auto_ptr<WriteLock> lock( new WriteLock(m_) );
  ReadTryLock tlock(m_);
  lock.reset();
  ensure("lock obtained, though lock's busy", !tlock.ownsLock() );
  ensure("lock not obtained (during call)", tlock.tryLock(0) );
}

// test if timed locking returns always true, when already locked
template<>
template<>
void testObj::test<10>(void)
{
  auto_ptr<WriteLock> lock( new WriteLock(m_) );
  ReadTryLock tlock(m_);
  lock.reset();
  ensure("lock obtained, though lock's busy", !tlock.ownsLock() );
  ensure("lock not obtained (during call)", tlock.tryLock(0) );
  ensure("lock not obtained", tlock.ownsLock() );
  ensure("locking fails, though lock is owned", tlock.tryLock(0) );
}

} // namespace tut
