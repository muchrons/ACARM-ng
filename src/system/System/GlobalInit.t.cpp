/*
 * GlobalInit.t.cpp
 *
 * NOTE: due to the nature of atexit() and related elements these tests, in
 *       most of the cases, only registe handles that will check proper
 *       conditions when program will quit, therefore if something's wrong
 *       usually asseration will fail uppon exit.
 */
#include <tut.h>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#include <cassert>

#include "System/GlobalInit.hpp"
#include "System/AtExit.hpp"

using namespace System;

namespace
{

enum State
{
  STATE_UNUSED,
  STATE_INITIALIZED,
  STATE_UNINITIALIZED
};

struct TestResult: public AtExitResourceDeallocator
{
  TestResult(int n, State *s):
    n_(n),
    s_(s)
  {
    tut::ensure("state cannot be NULL", s_!=NULL);
  }

  virtual void deallocate(void)
  {
    assert(s_!=NULL);
    tut::ensure( (boost::lexical_cast<std::string>(n_)+"#test failed").c_str(), *s_!=STATE_INITIALIZED);
  }

private:
  int    n_;
  State *s_;
}; // struct TestResult


struct TestClass
{
  static bool registerCheck(int n, State *s)
  {
    AtExit::TDeallocPtr test( new TestResult(n, s) );
    AtExit::registerDeallocator(test);
    return true;
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("System/GlobalInit");
} // unnamed namespace


namespace tut
{

// self-test 1
template<>
template<>
void testObj::test<1>(void)
{
  static State s=STATE_INITIALIZED;
  registerCheck(1, &s);
  s=STATE_UNUSED;
}

// self-test 2
template<>
template<>
void testObj::test<2>(void)
{
  static State s=STATE_INITIALIZED;
  registerCheck(2, &s);
  s=STATE_UNINITIALIZED;
}

namespace
{
static State g_stateNormal=STATE_UNUSED;

struct TestNormal
{
  static void init(void)
  {
    g_stateNormal=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateNormal=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
// test regular initialization/uninitialization
template<>
template<>
void testObj::test<3>(void)
{
  registerCheck(3, &g_stateNormal);
  GlobalInit<TestNormal> tmp;
}

namespace
{
static State g_stateExceptionOnInit=STATE_UNUSED;

struct TestExceptionOnInit
{
  static void init(void)
  {
    throw std::runtime_error("simulated error when init() is called");
  }
  static void uninit(void)
  {
    tut::fail("uninit() called when init() failed");
  }
};
} // unnamed namespace
// test if deinit is not called when exception is thrown in c-tor
template<>
template<>
void testObj::test<4>(void)
{
  registerCheck(4, &g_stateExceptionOnInit);
  try
  {
    GlobalInit<TestExceptionOnInit> tmp;
    fail("c-tor does not call init() at all");
  }
  catch(const std::runtime_error &)
  {
    // this is expected
  }
}

namespace
{
static State g_stateCopyCtor=STATE_UNUSED;

struct TestCopyCtor
{
  static void init(void)
  {
    ensure_equals("invalid initial state (init() not called or calledmultiple times)",
                  g_stateCopyCtor, STATE_UNUSED);
    g_stateCopyCtor=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateCopyCtor=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
template<>
template<>
void testObj::test<5>(void)
{
  registerCheck(5, &g_stateCopyCtor);
  GlobalInit<TestCopyCtor> tmp1;
  GlobalInit<TestCopyCtor> tmp2(tmp1);
}

namespace
{
static State g_stateAssignment=STATE_UNUSED;

struct TestAssignment
{
  static void init(void)
  {
    ensure_equals("invalid initial state (init() not called or calledmultiple times)",
                  g_stateAssignment, STATE_UNUSED);
    g_stateAssignment=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateAssignment=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
// test assignment
template<>
template<>
void testObj::test<6>(void)
{
  registerCheck(6, &g_stateAssignment);
  GlobalInit<TestAssignment> tmp1;
  GlobalInit<TestAssignment> tmp2;
  tmp2=tmp1;
}

namespace
{
static State g_stateMultipleInstances=STATE_UNUSED;

struct TestMultipleInstances
{
  static void init(void)
  {
    ensure_equals("invalid initial state (init() not called or calledmultiple times)",
                  g_stateMultipleInstances, STATE_UNUSED);
    g_stateMultipleInstances=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateMultipleInstances=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
// test multiple, independent instances
template<>
template<>
void testObj::test<7>(void)
{
  registerCheck(7, &g_stateMultipleInstances);
  GlobalInit<TestMultipleInstances> tmp1;
  GlobalInit<TestMultipleInstances> tmp2;
}

namespace
{
static State g_stateSelfAssignment=STATE_UNUSED;

struct TestSelfAssignment
{
  static void init(void)
  {
    ensure_equals("invalid initial state (init() not called or calledmultiple times)",
                  g_stateSelfAssignment, STATE_UNUSED);
    g_stateSelfAssignment=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateSelfAssignment=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
// test selft assignment
template<>
template<>
void testObj::test<8>(void)
{
  registerCheck(8, &g_stateSelfAssignment);
  GlobalInit<TestSelfAssignment> tmp;
  tmp=tmp;
}

namespace
{
static State g_stateInitLast=STATE_UNUSED;

struct TestInitLast
{
  static void init(void)
  {
    g_stateInitLast=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateInitLast=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
// test if uninit is NOT called when local object is destroyed
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("invalid initial state", g_stateInitLast, STATE_UNUSED);
  {
    GlobalInit<TestInitLast> tmp;
    ensure_equals("not initialized", g_stateInitLast, STATE_INITIALIZED);
  }
  ensure_equals("uninitialized too fast", g_stateInitLast, STATE_INITIALIZED);
}

namespace
{
static State g_stateGlobalInit=STATE_UNUSED;

struct TestGlobalInit
{
  static void init(void)
  {
    g_stateGlobalInit=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateGlobalInit=STATE_UNINITIALIZED;
  }
};

bool                       g_checkTestGlobalInit=TestClass::registerCheck(10, &g_stateGlobalInit);
GlobalInit<TestGlobalInit> g_testGlobalInit;
} // unnamed namespace
// test if global init works fine
template<>
template<>
void testObj::test<10>(void)
{
  ensure_equals("initialization failed", g_checkTestGlobalInit, true);
  ensure_equals("not initialized", g_stateGlobalInit, STATE_INITIALIZED);
}

namespace
{
static State g_stateChainInit1=STATE_UNUSED;
struct TestChainInit1
{
  static void init(void)
  {
    g_stateChainInit1=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateChainInit1=STATE_UNINITIALIZED;
  }
};

static State g_stateChainInit2=STATE_UNUSED;
struct TestChainInit2
{
  static void init(void)
  {
    GlobalInit<TestChainInit1> tmp1;        //require entity1
    g_stateChainInit2=STATE_INITIALIZED;
  }
  static void uninit(void)
  {
    g_stateChainInit2=STATE_UNINITIALIZED;
  }
};
} // unnamed namespace
// test if chain of inits works fine
template<>
template<>
void testObj::test<11>(void)
{
  registerCheck(11, &g_stateChainInit1);
  registerCheck(11, &g_stateChainInit2);

  // entity1 is initialized implicitly by entity2
  //GlobalInit<TestChainInit1> tmp1;
  GlobalInit<TestChainInit2> tmp2;

  ensure_equals("entity 1 not initialized", g_stateChainInit1, STATE_INITIALIZED);
  ensure_equals("entity 2 not initialized", g_stateChainInit2, STATE_INITIALIZED);
}

namespace
{
static int g_initCounter=0;
struct TestInitCounter
{
  static void init(void)
  {
    ++g_initCounter;
    // give some time for the second thread to work
    for(int i=0; i<1000; ++i)
      boost::this_thread::yield();
  }
  static void uninit(void)
  {
    assert( g_initCounter==1 );
    --g_initCounter;
  }
};

struct TestInitCounterThread
{
  void operator()(void)
  {
    GlobalInit<TestInitCounter> tmp;
  }
};
} // unnamed namespace
// test for locking when using from multiple threads
template<>
template<>
void testObj::test<12>(void)
{
  // created two threads that want to initialize data
  boost::thread th1( (TestInitCounterThread()) );
  boost::thread th2( (TestInitCounterThread()) );
  boost::thread th3( (TestInitCounterThread()) );
  // wait for them to finish
  th1.join();
  th2.join();
  th3.join();
  // check post-condition
  ensure_equals("race condition duruing initialization detected", g_initCounter, 1);
}

namespace
{
int g_sequenceInitCount=0;
struct TestSequenceInitCount
{
  static void init(void)
  {
    ++g_sequenceInitCount;
  }
  static void uninit(void)
  {
    --g_sequenceInitCount;
    assert( g_sequenceInitCount==0 );
  }
};
} // unnamed namespace
// test regular initialization/uninitialization
template<>
template<>
void testObj::test<13>(void)
{
  ensure_equals("already initialized", g_sequenceInitCount, 0);
  GlobalInit<TestSequenceInitCount> tmp1;
  ensure_equals("invalid ocunter after init 1", g_sequenceInitCount, 1);
  GlobalInit<TestSequenceInitCount> tmp2;
  ensure_equals("invalid ocunter after init 2", g_sequenceInitCount, 1);
}

namespace
{
int g_sequenceInitUninitCount=0;
struct TestSequenceInitUninitCount
{
  static void init(void)
  {
    ++g_sequenceInitUninitCount;
  }
  static void uninit(void)
  {
  }
};
} // unnamed namespace
// smoke tesr for re-initialization
template<>
template<>
void testObj::test<14>(void)
{
  ensure_equals("already initialized", g_sequenceInitUninitCount, 0);
  for(int i=0; i<3; ++i)
  {
    GlobalInit<TestSequenceInitUninitCount> tmp;
    ensure_equals( ("invalid counter value in loop#" + boost::lexical_cast<std::string>(i)).c_str(),
                   g_sequenceInitUninitCount, 1);
  }
}

namespace
{
int g_stateReinit=0;
struct TestReinitCheck: public AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    tut::ensure_equals("reinitialization does not work", g_stateReinit, 0x33);
  }
}; // struct TestReinitCheck
struct TestReinit;
struct ReinitDuringAtExit: public AtExitResourceDeallocator
{
  virtual void deallocate(void)
  {
    // register checker
    AtExit::TDeallocPtr test(new TestReinitCheck);
    AtExit::registerDeallocator(test);
    // re-init entity
    GlobalInit<TestReinit> tmp;
  }
}; // struct TestReinitCheck
struct TestReinit
{
  static void init(void)
  {
    if(g_stateReinit&0x01)
      g_stateReinit|=0x10;
    else
      g_stateReinit|=0x01;
  }
  static void uninit(void)
  {
    if(g_stateReinit&0x02)
      g_stateReinit|=0x20;
    else
    {
      g_stateReinit|=0x02;
      // register new creation to atexit - this will reinitialize entity
      AtExit::TDeallocPtr test(new ReinitDuringAtExit);
      AtExit::registerDeallocator(test);
    }
  }
};
struct TestReinitCtor
{
  TestReinitCtor(void)
  {
    GlobalInit<TestReinit> tmp;
  }
};
TestReinitCtor g_testReinitCtor;
} // unnamed namespace
// test re-initialization
template<>
template<>
void testObj::test<15>(void)
{
  ensure_equals("first initialization failed", g_stateReinit, 0x01);
}

} // namespace tut
