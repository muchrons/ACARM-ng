/*
 * ThreadIDMap.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Logger/ThreadIDMap.hpp"
#include "TestHelpers/TestBase.hpp"

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
  explicit IDGetter(ThreadIDMap *tim):
    tim_(tim)
  {
    ensure("pointer is NULL", tim_!=NULL);
  }

  void operator()(void)
  {
    assert(tim_!=NULL);
    ensure_equals("invalid ID returned for thread / 1", tim_->getThreadID(), 1u);   // add
    ensure_equals("invalid ID returned for thread / 2", tim_->getThreadID(), 1u);   // get
  }

private:
  ThreadIDMap *tim_;
}; // struct IDGetter
} // unnamed namespace

// test "get" from different thread
template<>
template<>
void testObj::test<4>(void)
{
  ThreadIDMap tim;
  ensure_equals("invalid ID returned / 1", tim.getThreadID(), 0u);  // add
  ensure_equals("invalid ID returned / 2", tim.getThreadID(), 0u);  // get
  // now do the same within a thread
  Base::Threads::ThreadJoiner tj( (IDGetter(&tim)) );
  tj->join();
}

} // namespace tut
