/*
 * LimitedQueue.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>
#include <boost/thread/thread.hpp>
#include <cstring>
#include <cassert>

#include "Base/Threads/LimitedQueue.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace std;
using namespace Base::Threads;

namespace
{
typedef LimitedQueue<std::string, 4u> LQ;

struct TestClass
{
  LQ q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/LimitedQueue");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  LQ q;
}

// test push/pop
template<>
template<>
void testObj::test<2>(void)
{
  q_.push("alice has a cat");
  const string &str=q_.pop();
  ensure_equals("invalid stirng", str, "alice has a cat");
}

namespace
{
struct CollectionWriter
{
  CollectionWriter(LQ *q, volatile int *state):
    q_(q),
    state_(state)
  {
    assert(q_    !=NULL);
    assert(state_!=NULL);
    assert(*state_==0);
  }

  void operator()(void)
  {
    q_->push("1st");
    q_->push("2nd");
    q_->push("3rd");
    q_->push("4th");
    *state_=1;
    q_->push("5th");    // this one blocks
    *state_=2;
  }

  LQ           *q_;
  volatile int *state_;
}; // struct CollectionWriter
} // unnamed namespace

// test blocking of push(), when queue is full
template<>
template<>
void testObj::test<3>(void)
{
  volatile int state=0;
  Base::Threads::ThreadJoiner th( CollectionWriter(&q_, &state) );
  // wait until thread blocks
  while(state!=1)
    boost::thread::yield();
  usleep(30*1000);          // ensure thread waits
  ensure_equals("invalid size", q_.size(), 4u);
  ensure_equals("invlid element read", q_.pop(), "1st");    // unblock queue
  th->join();
  ensure_equals("invalid state", state, 2);
  ensure_equals("invalid size after exti", q_.size(), 4u);
}

// test if top() does not unblock push()
template<>
template<>
void testObj::test<4>(void)
{
  volatile int state=0;
  Base::Threads::ThreadJoiner th( CollectionWriter(&q_, &state) );
  // wait until thread blocks
  while(state!=1)
    boost::thread::yield();
  usleep(30*1000);          // ensure thread waits
  ensure_equals("invalid size", q_.size(), 4u);
  ensure_equals("invlid element read", q_.top(), "1st");    // do NOT unblock queue
  usleep(30*1000);
  ensure_equals("invlid element read", q_.top(), "1st");    // do NOT unblock queue
  q_.pop();                 // unblock thread
  th->join();
  ensure_equals("invalid state", state, 2);
  ensure_equals("invalid size after exti", q_.size(), 4u);
}

namespace
{
struct Inserter
{
  explicit Inserter(LQ *q):
    q_(q)
  {
    assert(q_!=NULL);
  }

  void operator()(void)
  {
    for(int i=0; i<15*1000; ++i)
    {
      stringstream ss;
      ss<<"data from thread: "<<i;
      q_->push( ss.str() );
    }
  }

  LQ *q_;
}; // struct Inserter

struct Getter
{
  explicit Getter(LQ *q):
    q_(q)
  {
    assert(q_!=NULL);
  }

  void operator()(void)
  {
    for(int i=0; i<15*1000; ++i)
    {
      const string tmp=q_->pop();
      ensure( "invalid element returned - something's not right...",
              strstr( tmp.c_str(), "data from thread: ")!=NULL );
    }
  }

  LQ *q_;
}; // struct Getter
} // unnamed namespace

// do some stress-tests - many push/pops from concurrent threads
template<>
template<>
void testObj::test<5>(void)
{
  Base::Threads::ThreadJoiner th1( (Getter(&q_))   );
  Base::Threads::ThreadJoiner th2( (Getter(&q_))   );
  Base::Threads::ThreadJoiner th3( (Inserter(&q_)) );
  Base::Threads::ThreadJoiner th4( (Inserter(&q_)) );
  // wait for them to finish
  th1->join();
  th2->join();
  th3->join();
  th4->join();
}

// test order of push/pop
template<>
template<>
void testObj::test<6>(void)
{
  q_.push("1");
  q_.push("2");
  q_.push("3");
  ensure_equals("invalid element no. 1", q_.pop(), "1");
  ensure_equals("invalid element no. 2", q_.pop(), "2");
  ensure_equals("invalid element no. 3", q_.pop(), "3");
}

namespace
{
struct SignallerPop
{
  SignallerPop(LQ *q, volatile int *state):
    q_(q),
    state_(state)
  {
    assert(q_!=NULL);
    assert(state_!=NULL);
    assert(*state==0);
  }

  void operator()(void)
  {
    *state_=1;
    try
    {
      q_->pop();    // should block for a while and throw
      fail("pop() didn't throw on interrupt");
    }
    catch(const boost::thread_interrupted &)
    {
      // this is expected
    }
    *state_=2;
  }

  LQ           *q_;
  volatile int *state_;
}; // struct SignallerPop
} // unnamed namespace

// test signalAll() for pop()
template<>
template<>
void testObj::test<7>(void)
{
  int state=0;
  Base::Threads::ThreadJoiner th( SignallerPop(&q_, &state) );
  // wait until thread is ready
  while( state!=1 )
    boost::this_thread::yield();
  // wait a while to ensure other thread is waiting
  usleep(30*1000);
  // switch state and interrupt thread
  th->interrupt();
  q_.signalAll();
  // check if everything's fine.
  th->join();
  ensure_equals("invalid state", state, 2);
}

namespace
{
struct SignallerPush
{
  SignallerPush(LQ *q, volatile int *state):
    q_(q),
    state_(state)
  {
    assert(q_!=NULL);
    assert(state_!=NULL);
    assert(*state==0);
  }

  void operator()(void)
  {
    for(int i=0; i<4; ++i)
      q_->push("x");
    // next push will block
    *state_=1;
    try
    {
      q_->push("!block!");      // this call will block
      fail("push() didn't throw on interrupt");
    }
    catch(const boost::thread_interrupted &)
    {
      // this is expected
    }
    *state_=2;
  }

  LQ           *q_;
  volatile int *state_;
}; // struct SignallerPush
} // unnamed namespace

// test signalAll() for push()
template<>
template<>
void testObj::test<8>(void)
{
  int state=0;
  Base::Threads::ThreadJoiner th( SignallerPush(&q_, &state) );
  // wait until thread is ready
  while( state!=1 )
    boost::this_thread::yield();
  // wait a while to ensure other thread is waiting
  usleep(30*1000);
  // switch state and interrupt thread
  th->interrupt();
  q_.signalAll();
  // check if everything's fine.
  th->join();
  ensure_equals("invalid state", state, 2);
}

// test top() call
template<>
template<>
void testObj::test<9>(void)
{
  q_.push("str1");
  q_.push("str2");
  const LQ &q=q_;       // used to check for const top()
  // check
  ensure_equals("invalid stirng 1/1", q.top(), "str1");
  ensure_equals("invalid stirng 1/2", q.top(), "str1");
  q_.pop();
  ensure_equals("invalid stirng 2",   q_.pop(), "str2");
}

// test for value_type typedef's presence
template<>
template<>
void testObj::test<10>(void)
{
  q_.push("str1");
  LQ::value_type v=q_.pop();
  ensure_equals("invalid stirng", v, "str1");
}

} // namespace tut
