/*
 * GlobalLock.t.cpp
 *
 */
#include <tut.h>
#include <vector>
#include <sstream>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>

#include "Commons/Threads/Thread.hpp"
#include "PythonAPI/GlobalLock.hpp"

using namespace std;
using namespace PythonAPI;
using namespace Commons::Threads;

namespace
{
struct TestClass
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("PythonAPI/GlobalLock");
} // unnamed namespace

namespace tut
{

// test lock/unlock
template<>
template<>
void testObj::test<1>(void)
{
  GlobalLock e;
}

namespace
{
struct RWThread
{
  RWThread(std::stringstream &ss, size_t loops):
    ss_(&ss),
    loops_(loops)
  {
  }

  void operator()(void)
  {
    assert(ss_!=NULL);

    for(size_t i=0; i<loops_; ++i)
    {
      {
        GlobalLock lock;
        (*ss_)<<(i%10);
      }
      boost::thread::yield();
    }
  }

private:
  std::stringstream *ss_;
  size_t             loops_;
}; // struct RWThread
} // unnamed namespace

// test lock/unlock from multiple threads
template<>
template<>
void testObj::test<2>(void)
{
  std::stringstream                        ss;
  std::vector< boost::shared_ptr<Thread> > ths_;
  const size_t                             loops=1*1000;
  for(size_t i=0; i<10; ++i)
  {
    RWThread rw(ss, loops);
    boost::shared_ptr<Thread> th(new Thread(rw));
    ths_.push_back(th);
  }

  for(size_t i=0; i<ths_.size(); ++i)
    (*ths_[i])->join();

  ensure_equals("invalid string length", ss.str().length(), ths_.size()*loops);
}

} // namespace tut
