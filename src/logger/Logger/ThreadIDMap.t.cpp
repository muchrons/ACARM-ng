/*
 * ThreadIDMap.t.cpp
 *
 */
#include <tut.h>
#include <unistd.h>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Logger/ThreadIDMap.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/TimeoutChecker.hpp"

using namespace Logger;

namespace
{
struct TestClass: private TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Logger/ThreadIDMap");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  ThreadIDMap tim;
}

// test "get", that adds new entry (must start with zero)
template<>
template<>
void testObj::test<2>(void)
{
  ThreadIDMap tim;
  ensure_equals("invalid ID returned", tim.getThreadID(), 0u);
}

// test "get" that gets already existing entry
template<>
template<>
void testObj::test<3>(void)
{
  ThreadIDMap tim;
  ensure_equals("invalid ID returned / 1", tim.getThreadID(), 0u);  // add
  ensure_equals("invalid ID returned / 2", tim.getThreadID(), 0u);  // get
}


namespace
{
// helper class for checking IDs in test.
struct IDGetter
{
  IDGetter(ThreadIDMap *tim, const unsigned int *id, volatile int *state=NULL):
    state_(state),
    tim_(tim),
    id_(id)
  {
    ensure("pointer is NULL", tim_!=NULL);
  }

  void operator()(void)
  {
    assert(tim_!=NULL);
    const int id=tim_->getThreadID();
    if(id_!=NULL)
      ensure_equals("invalid ID value returned when adding", id, *id_);

    // extra wait requested?
    if(state_!=NULL)
    {
      *state_=1;
      const TestHelpers::TimeoutChecker tc(5);
      while( *state_!=2 && tc() )
        usleep(10*1000);
    }

    ensure_equals("invalid ID returned for thread", tim_->getThreadID(), id);  // get
  }

  volatile int       *state_;
private:
  ThreadIDMap        *tim_;
  const unsigned int *id_;
}; // struct IDGetter

void waitState(volatile int *state)
{
  const TestHelpers::TimeoutChecker tc(5);
  ensure("NULL state", state!=NULL);
  while( *state!=1 && tc() )
    usleep(11*1000);
} // waitState()
} // unnamed namespace


// test "get" from different thread
template<>
template<>
void testObj::test<4>(void)
{
  ThreadIDMap        tim;
  ensure_equals("invalid ID returned / 1", tim.getThreadID(), 0u);  // add
  ensure_equals("invalid ID returned / 2", tim.getThreadID(), 0u);  // get
  // now do the same within a thread
  const unsigned int id=1u;
  Base::Threads::ThreadJoiner tj( (IDGetter(&tim, &id)) );
  tj->join();
}

// test "get" from many threads, after some time
template<>
template<>
void testObj::test<5>(void)
{
  volatile int state[4]={0};
  ThreadIDMap  tim;
  // run multiple threads
  Base::Threads::ThreadJoiner tj1( (IDGetter(&tim, NULL, &state[0])) );
  waitState(&state[0]);
  Base::Threads::ThreadJoiner tj2( (IDGetter(&tim, NULL, &state[1])) );
  waitState(&state[1]);
  Base::Threads::ThreadJoiner tj3( (IDGetter(&tim, NULL, &state[2])) );
  waitState(&state[2]);
  Base::Threads::ThreadJoiner tj4( (IDGetter(&tim, NULL, &state[3])) );
  waitState(&state[3]);
  // proceed with next checks
  state[0]=2;
  state[1]=2;
  state[2]=2;
  state[3]=2;
  // wait for them
  tj1->join();
  tj2->join();
  tj3->join();
  tj4->join();
}

} // namespace tut
