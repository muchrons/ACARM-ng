/*
 * AtExitResourceDeallocator.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "System/AtExitResourceDeallocator.hpp"

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/AtExitResourceDeallocator");
}

using namespace std;
using namespace System;

// unnamed namespace for object deallocator testing
namespace
{
struct SomeTestObject
{
  explicit SomeTestObject(size_t *cnt):
    cnt_(cnt)
  {
    assert(cnt_!=NULL);
  }
  ~SomeTestObject(void)
  {
    assert(cnt_!=NULL);
    ++(*cnt_);
  }

private:
  size_t *cnt_;
}; // struct SomeTestObject
} // unnamed namespace


namespace tut
{

// smoke test - if this code does not work asseration will fail
// at the end of test binary execution.
template<>
template<>
void testObj::test<1>(void)
{
  typedef AtExitMemoryDeallocator<SomeTestObject> TDealloc;
  size_t counter=0;

  TDealloc dealloc( new SomeTestObject(&counter) );
  ensure_equals("counter changed in constructor", counter, 0);

  AtExitResourceDeallocator &base=dealloc;
  base.deallocate();
  ensure_equals("counter not changed - resource leaked", counter, 1);
}

// test creation of memory deallocator from auto_ptr.
template<>
template<>
void testObj::test<2>(void)
{
  typedef AtExitMemoryDeallocator<SomeTestObject> TDealloc;
  size_t counter=0;

  auto_ptr<SomeTestObject>   ptr( new SomeTestObject(&counter) );
  TDealloc                   dealloc(ptr);
  AtExitResourceDeallocator &base=dealloc;
  ensure_equals("counter changed in constructor", counter, 0);
  // now dealicaotion should take place
  base.deallocate();
  ensure_equals("counter didn't changed. object has not been freed",
                counter, 1);
}

} // namespace tut
