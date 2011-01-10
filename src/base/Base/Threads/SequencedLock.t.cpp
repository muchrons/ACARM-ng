/*
 * SequencedLock.t.cpp
 *
 */
#include <tut.h>
#include <boost/thread/thread.hpp>
#include <cassert>
#include <unistd.h>

#include "Base/Threads/SequencedLock.hpp"
#include "Base/Threads/ThreadJoiner.hpp"

using namespace Base::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Base/Threads/SequencedLock");
} // unnamed namespace


namespace tut
{

// basic lock/unlock
template<>
template<>
void testObj::test<1>(void)
{
  Mutex         m1;
  Mutex         m2;
  SequencedLock slock(m1, m2);
}

// re-locking
template<>
template<>
void testObj::test<2>(void)
{
  Mutex m1;
  Mutex m2;
  {
    SequencedLock slock(m1, m2);
  }
  // here mutex should be already released
  {
    SequencedLock slock(m1, m2);
  }
}

namespace
{

class TestLocker
{
public:
  TestLocker(Mutex *m1, Mutex *m2, double *data):
    mutex1_(m1),
    mutex2_(m2),
    data_(data)
  {
    assert(mutex1_!=NULL);
    assert(mutex2_!=NULL);
    assert(data_!=NULL);
  }
  void operator()(void)
  {
    const int seed=rand();
    for(int i=0; i<20; ++i)
    {
      SequencedLock lock(*mutex1_, *mutex2_);
      ensure_equals("data is invalid", data_[1], data_[0]+1);
      data_[0]=10.5+seed;
      boost::thread::yield();  // switch context
      data_[1]=data_[0]+1;
    }
  }
private:
  Mutex  *mutex1_;
  Mutex  *mutex2_;
  double *data_;
}; // class TestLocker

} // unnamed namespace

// test if locking works (i.e. check for races)
template<>
template<>
void testObj::test<3>(void)
{
  double     data[2]={1, 2};
  Mutex      mutex[2];
  // NOTE: following lockers are given mutexes in different order - if it
  //       waould not work, this would caused dead lock after run.
  TestLocker tl1(&mutex[1], &mutex[0], data);
  TestLocker tl2(&mutex[0], &mutex[1], data);

  // start two threads
  Base::Threads::ThreadJoiner th1(tl1);
  Base::Threads::ThreadJoiner th2(tl2);
  // and join them
  th1->join();
  th2->join();
}

} // namespace tut
