/*
 * AtExit.t.cpp
 *
 */
#include <tut.h>
#include <cassert>

#include "System/AtExit.hpp"

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;
factory tf("System/AtExit");
}

using namespace System;

// unnamed namespace for helper calss
namespace
{

struct TestDeallocator: public AtExitResourceDeallocator
{
  TestDeallocator(void):
    count_(0),
    ptr_(&count_)
  {
  }
  ~TestDeallocator(void)
  {
    assert(*ptr_==1 && "deallocation from System/AtExit "
                       "test 1 has not been called");
    ptr_=NULL;  // this will cause segv in case of access after
                // destructor has been called
  }
  virtual void deallocate(void)
  {
    ++(*ptr_);
  }

private:
  size_t  count_;
  size_t *ptr_;
}; // struct TestDealocator

} // unnamed namespace

namespace tut
{

// smoke test - if this code does not work asseration will fail
// at the end of test binary execution.
template<>
template<>
void testObj::test<1>(void)
{
  AtExit::TDeallocPtr test(new TestDeallocator);
  ensure("alocation failed", test.get()!=NULL);

  AtExit::registerDeallocator(test);
  ensure("ownership apssing failed", test.get()==NULL);
}

// test for exception when registering NULL pointer
template<>
template<>
void testObj::test<2>(void)
{
  AtExit::TDeallocPtr test(NULL);
  try
  {
    AtExit::registerDeallocator(test);
    fail("registration didn't throw exception on NULL pointer");
  }
  catch(const ExceptionPointerIsNULL &)
  {
    // this is expected
  }
}

} // namespace tut
