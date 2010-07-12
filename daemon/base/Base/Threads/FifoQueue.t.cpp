/*
 * FifoQueue.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <sstream>
#include <boost/thread/thread.hpp>
#include <cstring>
#include <cassert>

#include "Base/Threads/FifoQueue.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace std;
using namespace Base::Threads;

namespace
{
typedef FifoQueue<std::string> FQ;

struct TestClass
{
  FQ q_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/FifoQueue");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  FQ q;
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

// test order preservation
template<>
template<>
void testObj::test<3>(void)
{
  q_.push("str1");
  q_.push("str2");
  q_.push("str3");
  // check
  ensure_equals("invalid stirng 1", q_.pop(), "str1");
  ensure_equals("invalid stirng 2", q_.pop(), "str2");
  ensure_equals("invalid stirng 3", q_.pop(), "str3");
}

namespace
{
struct CollectionWriter
{
  CollectionWriter(FQ *q, int *state):
    q_(q),
    state_(state)
  {
    assert(q_!=NULL);
    assert(state_!=NULL);
    assert(*state==0 || *state==1);
  }

  void operator()(void)
  {
    // wait until main thread is ready
    while( *state_!=1 )
      boost::this_thread::yield();
    // wait a while to ensure other thread is waiting
    usleep(30*1000);
    // switch state and add element. order is important, not to cause races
    *state_=2;
    q_->push("new data");
  }

  FQ  *q_;
  int *state_;
}; // struct CollectionWriter
} // unnamed namespace

// test blocking of pop(), when no elements are present
template<>
template<>
void testObj::test<4>(void)
{
  int state=0;
  Base::Threads::ThreadJoiner th( CollectionWriter(&q_, &state) );
  state=1;
  const string &tmp=q_.pop();   // should block for a while
  th->join();
  ensure_equals("invalid state", state, 2);
  ensure_equals("invalid message", tmp, "new data");
}

namespace
{
struct Inserter
{
  explicit Inserter(FQ *q):
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

  FQ *q_;
}; // struct Inserter

struct Getter
{
  explicit Getter(FQ *q):
    q_(q)
  {
    assert(q_!=NULL);
  }

  void operator()(void)
  {
    for(int i=0; i<15*1000; ++i)
    {
      const string tmp=q_->pop();
      assert( strstr( tmp.c_str(), "data from thread: ")!=NULL &&
              "invalid element returned - something's not right..." );
    }
  }

  FQ *q_;
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

namespace
{
struct Signaller
{
  Signaller(FQ *q, int *state):
    q_(q),
    state_(state)
  {
    assert(q_!=NULL);
    assert(state_!=NULL);
    assert(*state==0 || *state==1);
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

  FQ  *q_;
  int *state_;
}; // struct CollectionWriter
} // unnamed namespace

// test blocking of pop(), when no elements are present
template<>
template<>
void testObj::test<6>(void)
{
  int state=0;
  Base::Threads::ThreadJoiner th( Signaller(&q_, &state) );
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
void testObj::test<7>(void)
{
  q_.push("str1");
  q_.push("str2");
  // check
  ensure_equals("invalid stirng 1/1", q_.top(), "str1");
  ensure_equals("invalid stirng 1/2", q_.top(), "str1");
  q_.pop();
  ensure_equals("invalid stirng 2",   q_.pop(), "str2");
}

} // namespace tut
