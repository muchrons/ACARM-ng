/*
 * MetaAlert.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/MetaAlert.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

struct IOMetaAlert: public IO::MetaAlert
{
public:
  IOMetaAlert(Persistency::MetaAlertPtrNN  ma,
              Transaction                 &t):
    IO::MetaAlert(ma, t),
    ma_(ma)
  {
    tut::ensure("invalid object saved", &get()==ma_.get() );

    for(unsigned int i=0; i<sizeof(calls_)/sizeof(calls_[0]); ++i)
      calls_[i]=0;
  }

  virtual void saveImpl(Transaction &)
  {
    ++calls_[0];
  }
  virtual void markAsTriggeredImpl(Transaction &, const std::string &)
  {
    ++calls_[7];
  }
  virtual void markAsUsedImpl(Transaction &)
  {
    ++calls_[1];
  }
  virtual void markAsUnusedImpl(Transaction &)
  {
    ++calls_[2];
  }
  virtual void updateSeverityDeltaImpl(Transaction &, double /*delta*/)
  {
    ++calls_[3];
  }
  virtual void updateCertaintyDeltaImpl(Transaction &, double /*delta*/)
  {
    ++calls_[4];
  }
  virtual void addChildImpl(Transaction &, Persistency::MetaAlertPtrNN /*child*/)
  {
    ++calls_[5];
  }
  virtual void associateWithAlertImpl(Transaction &, Persistency::AlertPtrNN /*alert*/)
  {
    ++calls_[6];
  }

  Persistency::MetaAlertPtr ma_;
  int                       calls_[8];
}; // class MetaAlert


struct TestClass: private TestBase
{
  TestClass(void):
    ma_( new Persistency::MetaAlert( makeNewAlert() ) ),
    tapi_( new TestTransactionAPI() ),
    t_(tapi_),
    ioma_(ma_, t_)
  {
  }

  void ensureCalls(unsigned int pos) const
  {
    for(unsigned int i=0; i<sizeof(ioma_.calls_)/sizeof(ioma_.calls_[0]); ++i)
      if(i==pos)
        tut::ensure_equals("invalid number of calls", ioma_.calls_[i], 1);
      else
        tut::ensure_equals("invalid call made", ioma_.calls_[i], 0);
  }

  Persistency::MetaAlertPtrNN ma_;
  TransactionAPIAutoPtr       tapi_;
  Transaction                 t_;
  IOMetaAlert                 ioma_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/MetaAlert");
} // unnamed namespace


namespace tut
{

// test creating from NULL
template<>
template<>
void testObj::test<1>(void)
{
  Persistency::MetaAlertPtr ma;
  assert( ma.get()==NULL );
  try
  {
    IOMetaAlert tmp(ma, t_);
    fail("NULL meta-alert has been accepted");
  }
  catch(const Commons::ExceptionUnexpectedNULL&)
  {
    // this is expected
  }
}

// test saving
template<>
template<>
void testObj::test<2>(void)
{
  ioma_.save();
  ensureCalls(0);
}

// test marking as used
template<>
template<>
void testObj::test<3>(void)
{
  ioma_.markAsUsed();
  ensureCalls(1);
}

// test marking as unused
template<>
template<>
void testObj::test<4>(void)
{
  ioma_.markAsUnused();
  ensureCalls(2);
}

// test severity delta
template<>
template<>
void testObj::test<5>(void)
{
  ioma_.updateSeverityDelta(0);
  ensureCalls(3);
}

// test certanity delta
template<>
template<>
void testObj::test<6>(void)
{
  ioma_.updateCertaintyDelta(0);
  ensureCalls(4);
}

// test adding child
template<>
template<>
void testObj::test<7>(void)
{
  ioma_.addChild(ma_);
  ensureCalls(5);
}

// test associating with alert
template<>
template<>
void testObj::test<8>(void)
{
  ioma_.associateWithAlert( makeNewAlert() );
  ensureCalls(6);
}

// test c-tor throw on inactive transaction
template<>
template<>
void testObj::test<9>(void)
{
  t_.rollback();
  try
  {
    IOMetaAlert tmp(ma_, t_);
    fail("c-tor didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if saving throws on inactive transaction
template<>
template<>
void testObj::test<10>(void)
{
  t_.rollback();
  try
  {
    ioma_.save();
    fail("save() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if markAsUnused() throws on inactive transaction
template<>
template<>
void testObj::test<11>(void)
{
  t_.rollback();
  try
  {
    ioma_.markAsUnused();
    fail("markAsUnused() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if markAsUsed() throws on inactive transaction
template<>
template<>
void testObj::test<12>(void)
{
  t_.rollback();
  try
  {
    ioma_.markAsUsed();
    fail("markAsUsed() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if updating severity throws on inactive transaction
template<>
template<>
void testObj::test<13>(void)
{
  t_.rollback();
  try
  {
    ioma_.updateSeverityDelta(4.2);
    fail("updateSeverityDelta() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if updating certanity throws on inactive transaction
template<>
template<>
void testObj::test<14>(void)
{
  t_.rollback();
  try
  {
    ioma_.updateCertaintyDelta(4.2);
    fail("updateCertaintyDelta() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if adding child throws on inactive transaction
template<>
template<>
void testObj::test<15>(void)
{
  t_.rollback();
  MetaAlertPtrNN mapnn( new Persistency::MetaAlert( makeNewAlert() ) );
  try
  {
    ioma_.addChild(mapnn);
    fail("addChild() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if associating with alert throws on inactive transaction
template<>
template<>
void testObj::test<16>(void)
{
  t_.rollback();
  try
  {
    ioma_.associateWithAlert( makeNewAlert() );
    fail("associateWithAlert() didn't throw on inactive transaction");
  }
  catch(const ExceptionTransactionNotActive&)
  {
    // this is expected
  }
}

// test if updating severity updates (not overwrites) value
template<>
template<>
void testObj::test<17>(void)
{
  double v=4;
  ioma_.updateSeverityDelta(v);
  ensure_equals("severity update 1 failed", ma_->getSeverityDelta(), v);

  v+=0.2;
  ioma_.updateSeverityDelta(0.2);
  ensure_equals("severity update 2 failed", ma_->getSeverityDelta(), v);
}

// test if updating certanity updates (not overwrites) value
template<>
template<>
void testObj::test<18>(void)
{
  double v=4;
  ioma_.updateCertaintyDelta(v);
  ensure_equals("certanity update 1 failed", ma_->getCertaintyDelta(), v);

  v+=0.2;
  ioma_.updateCertaintyDelta(0.2);
  ensure_equals("certanity update 2 failed", ma_->getCertaintyDelta(), v);
}

// test marking as triggered
template<>
template<>
void testObj::test<19>(void)
{
  ioma_.markAsTriggered("test");
  ensureCalls(7);
}

} // namespace tut
