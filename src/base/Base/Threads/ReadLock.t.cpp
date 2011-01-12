/*
 * ReadLock.t.cpp
 *
 */
#include <tut.h>

#include "Base/Threads/ReadLock.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/ReadLock");
} // unnamed namespace


namespace tut
{

// creating mutex type
template<>
template<>
void testObj::test<1>(void)
{
  ReadWriteMutex m;
  ReadLock       lock(m);
}

// re-locking
template<>
template<>
void testObj::test<2>(void)
{
  ReadWriteMutex m;
  {
    ReadLock lock(m);
  }
  // here mutex should be already released
  {
    ReadLock lock(m);
  }
}

// test if read locking works (if read lock does not work, this test will hang)
template<>
template<>
void testObj::test<3>(void)
{
  ReadWriteMutex mutex;
  ReadLock       lock1(mutex);
  ReadLock       lock2(mutex);
}

} // namespace tut
