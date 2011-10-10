/*
 * RecursiveLock.t.cpp
 *
 */
#include <tut.h>
#include <cassert>
#include <unistd.h>
#include <boost/scoped_ptr.hpp>
#include <boost/thread/thread.hpp>

#include "Base/Threads/RecursiveMutex.hpp"
#include "Base/Threads/RecursiveLock.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/RecursiveLock");
} // unnamed namespace


namespace tut
{

// creating mutex type
template<>
template<>
void testObj::test<1>(void)
{
  RecursiveMutex m;
  RecursiveLock  lock(m);
}

// re-locking
template<>
template<>
void testObj::test<2>(void)
{
  RecursiveMutex m;
  {
    RecursiveLock  lock(m);
  }
  // here mutex should be already released
  {
    RecursiveLock  lock(m);
  }
}


namespace
{
class TestLocker
{
public:
  TestLocker(RecursiveMutex *mutex, double *data):
    mutex_(mutex),
    data_(data)
  {
    assert(mutex_!=NULL);
    assert(data_!=NULL);
  }
  void operator()(void)
  {
    const int seed=rand();
    for(int i=0; i<200; ++i)
    {
      RecursiveLock lock(*mutex_);
      ensure_equals("data is invalid", data_[1], data_[0]+1);
      data_[0]=10.5+seed;
      boost::thread::yield();  // switch context
      data_[1]=data_[0]+1;
    }
  }
private:
  RecursiveMutex *mutex_;
  double         *data_;
}; // class TestLocker
} // unnamed namespace

// test if locking works (i.e. check for races)
template<>
template<>
void testObj::test<3>(void)
{
  double         data[2]={1, 2};
  RecursiveMutex mutex;
  TestLocker     tl1(&mutex, data);
  TestLocker     tl2(&mutex, data);

  // start two threads
  Base::Threads::ThreadJoiner th1(tl1);
  Base::Threads::ThreadJoiner th2(tl2);
  // and join them
  th1->join();
  th2->join();
}

// test relocking in the same thread
template<>
template<>
void testObj::test<4>(void)
{
  RecursiveMutex m;
  RecursiveLock  l1(m);
  RecursiveLock  l2(m);
}

} // namespace tut
