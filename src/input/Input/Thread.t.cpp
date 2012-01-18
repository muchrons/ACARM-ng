/*
 * Thread.t.cpp
 *
 */
#include <tut.h>
#include <unistd.h>

#include "Base/Threads/ThreadJoiner.hpp"
#include "Input/Thread.hpp"
#include "Input/TestConnection.t.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/TimeoutChecker.hpp"

using namespace std;
using namespace Input;
using namespace Core::Types::Proc;

namespace
{

struct TestReader: public Reader
{
  TestReader(void):
    Reader( TypeName("testreader"), InstanceName("testreadername") ),
    count_(0),
    justThrow_(false)
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    usleep(10*1000);   // limit output a little...
    ++count_;
    if(justThrow_)
      throw std::runtime_error("just-throw-test");
    return TestHelpers::Persistency::makeNewAlert().shared_ptr();
  }

  size_t count_;
  bool   justThrow_;
}; // struct TestReader

struct TestClass: public TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    tr_(new TestReader),
    r_(tr_),
    conn_( createUserStub() )
  {
  }

  TestReader                       *tr_;
  ReaderPtrNN                       r_;
  Core::Types::AlertsFifo           output_;
  Persistency::IO::ConnectionPtrNN  conn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Input/Thread");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Thread t(r_, conn_, output_);
}

// test running opertator() in separate thread and stopping it here
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-condition 1 failed", tr_->count_, 0u);
  ensure_equals("pre-condition 2 failed", output_.size(), 0u);
  {
    Base::Threads::ThreadJoiner th( Thread(r_, conn_, output_) );
    // run ~2-3 times
    const TestHelpers::TimeoutChecker tc(5);
    while( (tr_->count_==0 || tr_->count_==1 || tr_->count_==2) && tc() )
      usleep(10*1000);
    th->interrupt();
    th->join();
  }
  const size_t count=tr_->count_;
  usleep(100*1000);
  ensure_equals("thread didn't stopped", tr_->count_, count);
  ensure_equals("invalid queue size", output_.size(), count);
}

// test if throwing does not break thread
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("pre-condition failed", tr_->count_, 0u);
  tr_->justThrow_=true;
  {
    Base::Threads::ThreadJoiner th( Thread(r_, conn_, output_) );
    // run ~2-3 times
    while(tr_->count_==0 || tr_->count_==1 || tr_->count_==2)
      usleep(10*1000);
    th->interrupt();
    th->join();
  }
  const size_t count=tr_->count_;
  ensure("thread didn't run at all", count>0);
  usleep(100*1000);
  ensure_equals("thread didn't stopped", tr_->count_, count);
  ensure_equals("invalid queue size", output_.size(), 0u);
}


namespace
{
struct TestWaitingReader: public Reader
{
  TestWaitingReader(void):
    Reader( TypeName("testwaitingreader"), InstanceName("testwaitingreadername") )
  {
  }

  virtual DataPtr read(BackendFacade &, unsigned int)
  {
    usleep(10*1000);    // limit output a little...
    return DataPtr();   // return no results, so that read() on a thread will not exit
  }
}; // struct TestWaitingReader

struct WaitForInterrupt
{
  WaitForInterrupt(Core::Types::AlertsFifo *q, bool *done):
    q_(q),
    done_(done)
  {
  }

  void operator()(void)
  {
    Logger::Node log("test.waitforinterrupt");
    assert(q_!=NULL);
    try
    {
      LOGMSG_DEBUG(log, "waiting for message");
      q_->pop();
      LOGMSG_DEBUG(log, "got message");
    }
    catch(const boost::thread_interrupted &)
    {
      LOGMSG_DEBUG(log, "got interrupt");
      assert(done_!=NULL);
      *done_=true;
    }
    LOGMSG_DEBUG(log, "exiting thread");
  }

  Core::Types::AlertsFifo *q_;
  bool                    *done_;
}; // struct WaitForInterrupt
} // unnamed namespace

// test if queue is signaled uppon interruption
template<>
template<>
void testObj::test<4>(void)
{
  bool                        done=false;
  ReaderPtrNN                 r(new TestWaitingReader);
  const Thread                pt(r, conn_, output_);
  const WaitForInterrupt      wfi(&output_, &done);
  Base::Threads::ThreadJoiner thInt(wfi);       // run waiting thread
  Base::Threads::ThreadJoiner th(pt);           // run generating thread.

  ensure("oops - thread that supposed to wait finised earlier", done==false);

  // now check...
  th->interrupt();      // stop processing thread
  thInt->interrupt();   // mark thread as ready-to-exit
  thInt->join();        // wait until thread waiting for data is done.
  ensure("thread didn't join?!", done==true);
  th->join();           // wait for ptocessing thread
}

} // namespace tut
