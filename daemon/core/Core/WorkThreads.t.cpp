/*
 * main.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <boost/thread.hpp>

#include "Core/WorkThreads.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Core;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/WorkThreads");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  WorkThreads m;
}

// test stopping request and joining
template<>
template<>
void testObj::test<2>(void)
{
  WorkThreads m;
  m.stop();
  m.waitUntilDone();
}


namespace
{
struct Stopper
{
  Stopper(volatile bool *ready, WorkThreads *m):
    ready_(ready),
    m_(m)
  {
  }

  void operator()(void)
  {
    assert(ready_!=NULL);
    while(!*ready_)
      usleep(30*1000);
    usleep(50*1000);
    assert(m_!=NULL);
    m_->stop();
  }

  volatile bool *ready_;
  WorkThreads          *m_;
}; // struct Stopper
} // unnamed namespace

// test interruption form other thread.
template<>
template<>
void testObj::test<3>(void)
{
  volatile bool ready=false;
  WorkThreads          m;
  boost::thread th( Stopper(&ready, &m) );
  ready=true;
  m.waitUntilDone();
}

} // namespace tut
