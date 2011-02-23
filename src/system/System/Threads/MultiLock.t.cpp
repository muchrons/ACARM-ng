/*
 * MultiLock.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <boost/noncopyable.hpp>

#include "System/Threads/MultiLock.hpp"

using namespace System::Threads;

namespace
{

int g_counter;

struct TestMutex: private boost::noncopyable
{
  int i_;
}; // struct TestMutex

struct TestLock: private boost::noncopyable
{
  explicit TestLock(TestMutex &m)
  {
    m.i_=++g_counter;
  }
}; // struct TestLock


struct TestClass
{
  typedef MultiLock<TestMutex, TestLock> TestMultiLock;

  TestClass(void)
  {
    g_counter=0;
  }

  // table ensures order within the memory
  TestMutex m[2];
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("System/Threads/MultiLock");
} // unnamed namespace


namespace tut
{

// test locking in forward order
template<>
template<>
void testObj::test<1>(void)
{
  TestMultiLock tml(m[0], m[1]);
  ensure_equals("invalid lock order for mutex 1", m[0].i_, 1);
  ensure_equals("invalid lock order for mutex 2", m[1].i_, 2);
}

// test locking in rewerse order
template<>
template<>
void testObj::test<2>(void)
{
  TestMultiLock tml(m[1], m[0]);
  ensure_equals("invalid lock order for mutex 1", m[0].i_, 1);
  ensure_equals("invalid lock order for mutex 2", m[1].i_, 2);
}

} // namespace tut
