/*
 * CUrlInit.t.cpp
 *
 */
#include <tut.h>
#include <list>
#include <boost/checked_delete.hpp>
#include <cassert>

#include "System/AtExit.hpp"
#include "Filter/IPBlackList/CUrlInit.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

CUrlInit *g_init=NULL;

struct TestDeallocator: public System::AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    boost::checked_delete(g_init);
    g_init=NULL;
  }
}; // struct TestDeallocator


// there was a bug with deinitialization uppon exit, when there are still
// instances fo CUrlInit running. this call reproduces this bug.
// it passes, when code does not crash with assertion/etc...
//
// NOTE: this code must be executed exactly once, before instantiating
//       CUrlInit object - this ensures that it will be deallocated
//       before last instance exits, thus we can check distributed
//       ownership feature.
bool initTestForBug(void)
{
  System::AtExit::TDeallocPtr ptr(new TestDeallocator);
  System::AtExit::registerDeallocator(ptr);
  g_init=new CUrlInit;
  assert(g_init!=NULL);
  return true;
} // initTestForBug()

const bool g_initDone=initTestForBug();


struct TestClass: private TestHelpers::Persistency::TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/CUrlInit");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  CUrlInit tmp;
}

// test multiple instances
template<>
template<>
void testObj::test<2>(void)
{
  CUrlInit tmp1;
  CUrlInit tmp2;
}

// ensure that code is ready for test for a bug in deinitialization uppon exit.
template<>
template<>
void testObj::test<3>(void)
{
  ensure("initialization for bug-test not done", g_initDone==true);
}

} // namespace tut
