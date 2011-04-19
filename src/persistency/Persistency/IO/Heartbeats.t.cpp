/*
 * Heartbeats.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Heartbeats.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

struct IOHeartbeats: public IO::Heartbeats
{
public:
  IOHeartbeats(const Owner &owner, Transaction &t):
    IO::Heartbeats(owner, t),
    reportedCalls_(0)
  {
  }

  virtual void reportImpl(Transaction &/*t*/, const Owner &o, const Module &m, Timestamp reported, time_t timeout)
  {
    ++reportedCalls_;
    paramOwner_   =o;
    paramModule_  =m;
    paramReported_=reported;
    paramTimeout_ =timeout;
  }

  Owner     paramOwner_;
  Module    paramModule_;
  Timestamp paramReported_;
  time_t    paramTimeout_;
  size_t    reportedCalls_;
}; // class Heartbeats


struct TestClass: private TestBase
{
  TestClass(void):
    tapi_( new TestTransactionAPI() ),
    t_(tapi_),
    ioh_("Mr.Evil", t_)
  {
  }

  TransactionAPIAutoPtr tapi_;
  Transaction           t_;
  IOHeartbeats          ioh_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Heartbeats");
} // unnamed namespace


namespace tut
{

// test creating from NULL
template<>
template<>
void testObj::test<1>(void)
{
  try
  {
    IOHeartbeats tmp(NULL, t_);
    fail("NULL owner has been accepted");
  }
  catch(const Commons::ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test reporting something
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-condition failed", ioh_.reportedCalls_, 0u);
  // test
  const Timestamp before=Timestamp();
  ioh_.report("MoD", 42u);
  const Timestamp after =Timestamp();
  // check
  ensure_equals("implementation not called", ioh_.reportedCalls_, 1u);
  ensure_equals("invalid owner passed", ioh_.paramOwner_.get(), string("Mr.Evil") );
  ensure_equals("invalid module passed", ioh_.paramModule_.get(), string("MoD") );
  ensure("timestamp is too soon", ioh_.paramReported_.get()>=before.get() );
  ensure("timestamp is too late", ioh_.paramReported_.get()<=after.get() );
  ensure_equals("invalid timestamp passed", ioh_.paramTimeout_, 42u);
}

// test exception on too small timeout
template<>
template<>
void testObj::test<3>(void)
{
  try
  {
    ioh_.report("my module", 0u);
    fail("too invalid timeout has been accepted");
  }
  catch(const IO::Exception &)
  {
    // this is expected
  }
}

} // namespace tut
