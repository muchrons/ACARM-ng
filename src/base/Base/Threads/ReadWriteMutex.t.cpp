/*
 * ReadWriteMutex.t.cpp
 *
 */
#include <tut.h>
#include <sstream>
#include <boost/thread/thread.hpp>
#include <ctime>
#include <cassert>
#include <unistd.h>

#include "Base/Threads/ReadWriteMutex.hpp"
#include "Base/Threads/ReadLock.hpp"
#include "Base/Threads/WriteLock.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/ReadWriteMutex");
} // unnamed namespace


namespace tut
{

// creating mutex type
template<>
template<>
void testObj::test<1>(void)
{
  ReadWriteMutex m;
}

namespace
{

class TestWriter
{
public:
  TestWriter(ReadWriteMutex *mutex, double *data):
    mutex_(mutex),
    data_(data)
  {
    assert(mutex_!=NULL);
    assert(data_!=NULL);
  }
  void operator()(void)
  {
    const int seed=rand();
    for(int i=0; i<200; ++i)
    {
      WriteLock lock(*mutex_);
      ensure_equals("data is invalid", data_[1], data_[0]+1);
      data_[0]=10.5+seed;
      boost::thread::yield();  // switch context
      data_[1]=data_[0]+1;
    }
  }
private:
  ReadWriteMutex *mutex_;
  double         *data_;
}; // class TestWriter

class TestReader
{
public:
  TestReader(ReadWriteMutex *mutex, double *data):
    mutex_(mutex),
    data_(data)
  {
    assert(mutex_!=NULL);
    assert(data_!=NULL);
  }
  void operator()(void)
  {
    for(int i=0; i<200; ++i)
    {
      boost::thread::yield();  // switch context
      ReadLock lock(*mutex_);
      ensure_equals("data is invalid", data_[1], data_[0]+1);
    }
  }
private:
  ReadWriteMutex *mutex_;
  double         *data_;
}; // class TestReader

} // unnamed namespace

// test if reader/writer switching works
template<>
template<>
void testObj::test<2>(void)
{
  double         data[2]={1, 2};
  ReadWriteMutex mutex;
  TestReader     tl1(&mutex, data);
  TestWriter     tl2(&mutex, data);
  TestReader     tl3(&mutex, data);
  TestWriter     tl4(&mutex, data);

  // start two threads
  Base::Threads::ThreadJoiner th1(tl1);
  Base::Threads::ThreadJoiner th2(tl2);
  Base::Threads::ThreadJoiner th3(tl3);
  Base::Threads::ThreadJoiner th4(tl4);
  // and join them
  th1->join();
  th2->join();
  th3->join();
  th4->join();
}


namespace
{

class TestReader2
{
public:
  TestReader2(ReadWriteMutex *mutex, volatile int *state, volatile bool *ok):
    mutex_(mutex),
    state_(state),
    ok_(ok)
  {
    assert(mutex_!=NULL);
    assert(state_!=NULL);
    assert(ok_!=NULL);
    assert(*ok_==false);
  }
  void operator()(void)
  {
    // initialize read-locks
    {
      ReadLock lock(*mutex_);
      wait(1);
      wait(2);
    }
    // read-locks released - wait until write will be locked
    wait(3);
    // grab read locks once again
    *state_=4;
    ReadLock lock(*mutex_);
    wait(5);
  }
private:
  void wait(const int state)
  {
    *ok_=false;                 // reset flag
    *state_=state;              // move to next state
    while(*ok_==false)          // wait until main program reached state
      boost::thread::yield();   // switch context
    *ok_=false;                 // reset flag
  }
  ReadWriteMutex *mutex_;
  volatile int   *state_;
  volatile bool  *ok_;
}; // class TestReader2

class TestWriter2
{
public:
  TestWriter2(ReadWriteMutex *mutex, volatile int *state, volatile bool *ok):
    mutex_(mutex),
    state_(state),
    ok_(ok)
  {
    assert(mutex_!=NULL);
    assert(state_!=NULL);
    assert(ok_!=NULL);
    assert(*ok_==false);
  }
  void operator()(void)
  {
    // initialize write lock
    {
      wait(1);
      *state_=2;
      WriteLock lock(*mutex_);
      wait(3);
      wait(4);
    }
    wait(5);
  }
private:
  void wait(const int state)
  {
    *ok_=false;                 // reset flag
    *state_=state;              // move to next state
    while(*ok_==false)          // wait until main program reached state
      boost::thread::yield();   // switch context
    *ok_=false;                 // reset flag
  }
  ReadWriteMutex *mutex_;
  volatile int   *state_;
  volatile bool  *ok_;
}; // class TestReader2

void waitForStates(int s1, int s2, int s3, volatile int *state, volatile bool *ok)
{
  const time_t timeout=time(NULL)+5;    // give ~5[s] timeout

  // wait for a given state
  while(state[0]!=s1 || state[1]!=s2 || state[2]!=s3)
  {
    boost::thread::yield();
    if( time(NULL)>timeout )
    {
      std::stringstream ss;
      ss << "timeout while waiting for state "
            "<" << s1       << "," << s2       << "," << s3       << "> - got "
            "<" << state[0] << "," << state[1] << "," << state[2] << ">";
      fail( ss.str().c_str() );
    }
    usleep(20*1000);
  }

  // proceed to next step
  for(int i=0; i<3; ++i)
    ok[i]=true;
}

} // unnamed namespace

// test if multiple readers are possible and if write is exclusive
template<>
template<>
void testObj::test<3>(void)
{
  static volatile bool  ok[3]   ={false, false, false};
  static volatile int   state[3]={0, 0, 0};
  static ReadWriteMutex mutex;
  // writer
  TestWriter2    tl1(&mutex, &state[0], &ok[0]);
  // readers
  TestReader2    tl2(&mutex, &state[1], &ok[1]);
  TestReader2    tl3(&mutex, &state[2], &ok[2]);

  // start two threads
  Base::Threads::ThreadJoiner th1(tl1);
  Base::Threads::ThreadJoiner th2(tl2);
  Base::Threads::ThreadJoiner th3(tl3);

  // wait until reading mutexes are locked
  waitForStates(1, 1, 1, state, ok);
  // wait for write mutex to hang (blocked by readers)
  waitForStates(2, 2, 2, state, ok);
  // wait for write mutex to lock
  waitForStates(3, 3, 3, state, ok);
  // wait for readers locks to block while writer is locked
  waitForStates(4, 4, 4, state, ok);
  // wait fo writer mutex to release and lock reading ones
  waitForStates(5, 5, 5, state, ok);

  // and join them
  th1->join();
  th2->join();
  th3->join();
}

} // namespace tut
