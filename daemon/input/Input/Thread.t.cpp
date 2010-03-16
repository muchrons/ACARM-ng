/*
 * Thread.t.cpp
 *
 */
#include <tut.h>
#include <unistd.h>

#include "Input/Thread.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Input;

namespace
{

struct TestReader: public Reader
{
  TestReader(void):
    Reader("testreader"),
    count_(0),
    justThrow_(false)
  {
  }

  virtual DataPtr read(unsigned int)
  {
    usleep(50*1000);   // limit output a little...
    ++count_;
    if(justThrow_)
      throw std::runtime_error("just-throw-test");
    return TestHelpers::Persistency::makeNewAlert();
  }

  int  count_;
  bool justThrow_;
}; // struct TestReader

struct TestClass
{
  TestClass(void):
    tr_(new TestReader),
    r_(tr_)
  {
  }

  TestReader              *tr_;
  ReaderPtrNN              r_;
  Core::Types::AlertsFifo  output_;
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
  Thread t(r_, output_);
}

// test running opertator() in separate thread and stopping it here
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-condition 1 failed", tr_->count_, 0);
  ensure_equals("pre-condition 2 failed", output_.size(), 0);
  {
    boost::thread th( Thread(r_, output_) );
    usleep(130*1000);   // run ~2-3 times
    th.interrupt();
    th.join();
  }
  const int count=tr_->count_;
  usleep(100*1000);
  ensure_equals("thread didn't stopped", tr_->count_, count);
  ensure_equals("invalid queue size", output_.size(), count);
}

// test if throwing does not break thread
template<>
template<>
void testObj::test<3>(void)
{
  ensure_equals("pre-condition failed", tr_->count_, 0);
  tr_->justThrow_=true;
  {
    boost::thread th( Thread(r_, output_) );
    usleep(130*1000);   // run ~2-3 times
    th.interrupt();
    th.join();
  }
  const int count=tr_->count_;
  ensure("thread didn't run at all", count>0);
  usleep(100*1000);
  ensure_equals("thread didn't stopped", tr_->count_, count);
  ensure_equals("invalid queue size", output_.size(), 0);
}

} // namespace tut
