/*
 * Connection.t.cpp
 *
 */
#include <tut.h>
#include <sstream>

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

using namespace std;
using namespace tut;
using namespace Persistency;
using namespace Persistency::IO;

namespace
{

class IOAlert: public IO::Alert
{
public:
  IOAlert(Persistency::AlertPtrNN  alert,
          const Transaction       &t):
    IO::Alert(alert, t),
    alert_(alert),
    calls_(0)
  {
  }

  virtual void saveImpl(void)
  {
    ++calls_;
    ensure("invalid object", &get()==alert_.get() );
  }

  Persistency::AlertPtrNN alert_;
  int                     calls_;
}; // class IOAlert


class IOHost: public IO::Host
{
public:
  IOHost(Persistency::HostPtrNN  host,
         const Transaction      &t):
    IO::Host(host, t),
    host_(host)
  {
  }

  virtual void setNameImpl(const Persistency::Host::Name &/*name*/)
  {
    ++calls_;
    ensure("invalid host", &get()==host_.get() );
  }

  Persistency::HostPtr host_;
  int                  calls_;
}; // class IOHost

class IOMetaAlert: public IO::MetaAlert
{
public:
  IOMetaAlert(Persistency::MetaAlertPtrNN  ma,
              const Transaction           &t):
    IO::MetaAlert(ma, t),
    ma_(ma)
  {
    for(unsigned int i=0; i<sizeof(called_)/sizeof(called_[0]); ++i)
      called_[i]=0;
  }

  virtual void saveImpl(void)
  {
    ++called_[0];
    ensure("invalid meta alert", &get()==ma_.get() );
  }
  virtual void markAsUsedImpl(void)
  {
    ++called_[1];
    ensure("invalid meta alert", &get()==ma_.get() );
  }
  virtual void markAsUnusedImpl(void)
  {
    ++called_[2];
    ensure("invalid meta alert", &get()==ma_.get() );
  }
  virtual void updateSeverityDeltaImpl(double /*delta*/)
  {
    ++called_[3];
    ensure("invalid meta alert", &get()==ma_.get() );
  }
  virtual void updateCertanityDeltaImpl(double /*delta*/)
  {
    ++called_[4];
    ensure("invalid meta alert", &get()==ma_.get() );
  }
  virtual void addChildImpl(Persistency::MetaAlertPtrNN /*child*/)
  {
    ++called_[5];
    ensure("invalid meta alert", &get()==ma_.get() );
  }
  virtual void associateWithAlertImpl(Persistency::AlertPtrNN /*alert*/)
  {
    ++called_[6];
    ensure("invalid meta alert", &get()==ma_.get() );
  }

  Persistency::MetaAlertPtr ma_;
  int                       called_[7];
}; // class MetaAlert


struct TestIOConnection: public Persistency::IO::Connection
{
  TestIOConnection(void)
  {
    for(unsigned int i=0; i<sizeof(called_)/sizeof(called_[0]); ++i)
      called_[i]=0;
  }

  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &/*mutex*/,
                                                         const std::string    &/*name*/)
  {
    ++called_[0];
    return TransactionAPIAutoPtr(new TestTransactionAPI);
  }
  virtual AlertAutoPtr alertImpl(AlertPtrNN alert, const Transaction &t)
  {
    ++called_[1];
    return AlertAutoPtr( new IOAlert(alert, t) );
  }
  virtual HostAutoPtr hostImpl(HostPtrNN host, const Transaction &t)
  {
    ++called_[2];
    return HostAutoPtr( new IOHost(host, t) );
  }
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, const Transaction &t)
  {
    ++called_[3];
    return MetaAlertAutoPtr( new IOMetaAlert(ma, t) );
  }

  int called_[4];
}; // struct TestIOConnection

struct TestClass
{
  void ensureCalls(unsigned int pos, int count=1)
  {
    for(unsigned int i=0; i<sizeof(c_.called_)/sizeof(c_.called_[0]); ++i)
      if(i==0)
        ensure_equals("transaction has not been made", c_.called_[0], 1);
      else
        if(i!=pos)
        {
          stringstream ss;
          ss<<"invalid call has been made to call number "<<i;
          ensure_equals( ss.str().c_str(), c_.called_[i], 0);
        }
        else
          ensure_equals("call has not been made given number of times",
                        c_.called_[i], count);
  }

  TestIOConnection c_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Connection");
} // unnamed namespace


namespace tut
{

// test creating new transaction
template<>
template<>
void testObj::test<1>(void)
{
  IO::TransactionAPIAutoPtr tapi=c_.createNewTransaction("X");
  ensure("NULL pointer retuirned", tapi.get()!=NULL );
  ensureCalls(0);
}

// test creating alert-io
template<>
template<>
void testObj::test<2>(void)
{
  const Transaction t( c_.createNewTransaction("abc") );
  AlertAutoPtr ptr=c_.alert( makeNewAlert(), t);
  ensure("NULL pointer received", ptr.get()!=NULL );
  ensureCalls(1);
}

// test creating host-io
template<>
template<>
void testObj::test<3>(void)
{
  const Transaction t( c_.createNewTransaction("abc") );
  HostAutoPtr ptr=c_.host( makeNewHost(), t);
  ensure("NULL pointer received", ptr.get()!=NULL );
  ensureCalls(2);
}

// test creating meta-alert-io
template<>
template<>
void testObj::test<4>(void)
{
  const Transaction t( c_.createNewTransaction("abc") );
  AlertPtrNN   alert=makeNewAlert();
  MetaAlertPtr ma( new Persistency::MetaAlert(alert) );
  MetaAlertAutoPtr ptr=c_.metaAlert(ma, t);
  ensure("NULL pointer received", ptr.get()!=NULL );
  ensureCalls(3);
}

} // namespace tut
