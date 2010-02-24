/*
 * SafeInitLocking.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <errno.h>

#include "System/Threads/SafeInitLocking.hpp"

using namespace System::Threads;

namespace
{

// smoke test for global initialization - it should not crash
// the test application.
SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_testMutex);
SafeInitLock g_testLock(g_testMutex);

SYSTEM_MAKE_STATIC_SAFEINIT_MUTEX(g_mutex);

struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Threads/SafeInitLocking");
} // unnamed namespace


namespace tut
{

// test basic locking/unlocking
template<>
template<>
void testObj::test<1>(void)
{
  SafeInitLock lock(g_mutex);
}

// check if global mutex is locked
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("mutex not locked", pthread_mutex_trylock(&g_testMutex), EBUSY);
}

// test multiple locking in a row
template<>
template<>
void testObj::test<3>(void)
{
  for(int i=0; i<3; ++i)
    SafeInitLock lock(g_mutex);
}

// check typedef
template<>
template<>
void testObj::test<4>(void)
{
  SafeInitLock::MutexType &mt=g_mutex;
  SafeInitLock lock(mt);
}

} // namespace tut
