/*
 * Lock.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread/thread.hpp>
#include <cassert>
#include <unistd.h>

#include "Commons/Threads/Lock.hpp"

using namespace Commons::Threads;

namespace
{
struct TestClass
{
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Commons/Threads/Lock");
} // unnamed namespace


namespace tut
{

// creating mutex type
template<>
template<>
void testObj::test<1>(void)
{
  Mutex m;
  Lock  lock(m);
}

// re-locking
template<>
template<>
void testObj::test<2>(void)
{
  Mutex m;
  {
    Lock  lock(m);
  }
  // here mutex should be already released
  {
    Lock  lock(m);
  }
}

namespace
{

class TestLocker
{
public:
  TestLocker(Mutex *mutex, double *data):
    mutex_(mutex),
    data_(data)
  {
    assert(mutex!=NULL);
    assert(data!=NULL);
  }
  void operator()(void)
  {
    const int seed=rand();
    for(int i=0; i<20; ++i)
    {
      Lock lock(*mutex_);
      ensure_equals("data is invalid", data_[1], data_[0]+1);
      data_[0]=10.5+seed;
      usleep(2*1000);       // wait 2[ms] (i.e. switch context)
      data_[1]=data_[0]+1;
    }
  }
private:
  Mutex  *mutex_;
  double *data_;
}; // class TestLocker

} // unnamed namespace

// test if locking works (i.e. check for races)
template<>
template<>
void testObj::test<3>(void)
{
  double     data[2]={1, 2};
  Mutex      mutex;
  TestLocker tl1(&mutex, data);
  TestLocker tl2(&mutex, data);

  // start two threads
  boost::thread th1(tl1);
  boost::thread th2(tl2);
  // and join them
  th1.join();
  th2.join();
}

} // namespace tut
