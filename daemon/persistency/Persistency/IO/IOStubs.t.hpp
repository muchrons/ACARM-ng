/*
 * IOStubs.t.cpp
 *
 */
#ifndef INCLUDE_IOSTUBS_T_HPP_FILE
#define INCLUDE_IOSTUBS_T_HPP_FILE

#include <tut.h>

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

namespace
{

class IOAlert: public Persistency::IO::Alert
{
public:
  IOAlert(Persistency::AlertPtrNN             alert,
          const Persistency::IO::Transaction &t):
    Persistency::IO::Alert(alert, t),
    alert_(alert),
    calls_(0)
  {
  }

  virtual void saveImpl(void)
  {
    ++calls_;
    tut::ensure("invalid object", &get()==alert_.get() );
  }

  Persistency::AlertPtrNN alert_;
  int                     calls_;
}; // class IOAlert


class IOHost: public Persistency::IO::Host
{
public:
  IOHost(Persistency::HostPtrNN              host,
         const Persistency::IO::Transaction &t):
    Persistency::IO::Host(host, t),
    host_(host)
  {
  }

  virtual void setNameImpl(const Persistency::Host::Name &/*name*/)
  {
    ++calls_;
    tut::ensure("invalid host", &get()==host_.get() );
  }

  Persistency::HostPtr host_;
  int                  calls_;
}; // class IOHost

class IOMetaAlert: public Persistency::IO::MetaAlert
{
public:
  IOMetaAlert(Persistency::MetaAlertPtrNN         ma,
              const Persistency::IO::Transaction &t):
    Persistency::IO::MetaAlert(ma, t),
    ma_(ma)
  {
    tut::ensure("invalid meta alert", &get()==ma_.get() );

    for(unsigned int i=0; i<sizeof(called_)/sizeof(called_[0]); ++i)
      called_[i]=0;
  }

  virtual void saveImpl(void)
  {
    ++called_[0];
  }
  virtual void markAsUsedImpl(void)
  {
    ++called_[1];
  }
  virtual void markAsUnusedImpl(void)
  {
    ++called_[2];
  }
  virtual void updateSeverityDeltaImpl(double /*delta*/)
  {
    ++called_[3];
  }
  virtual void updateCertanityDeltaImpl(double /*delta*/)
  {
    ++called_[4];
  }
  virtual void addChildImpl(Persistency::MetaAlertPtrNN /*child*/)
  {
    ++called_[5];
  }
  virtual void associateWithAlertImpl(Persistency::AlertPtrNN /*alert*/)
  {
    ++called_[6];
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

  virtual Persistency::IO::TransactionAutoPtr createNewTransactionImpl(
                                        Base::Threads::Mutex &/*mutex*/,
                                        const std::string    &/*name*/)
  {
    ++called_[0];
    return Persistency::IO::TransactionAutoPtr(new TestTransactionAPI);
  }
  virtual Persistency::IO::AlertAutoPtr alertImpl(
                                        Persistency::AlertPtrNN             alert,
                                        const Persistency::IO::Transaction &t)
  {
    ++called_[1];
    return Persistency::IO::AlertAutoPtr( new IOAlert(alert, t) );
  }
  virtual Persistency::IO::HostAutoPtr hostImpl(
                                        Persistency::HostPtrNN              host,
                                        const Persistency::IO::Transaction &t)
  {
    ++called_[2];
    return Persistency::IO::HostAutoPtr( new IOHost(host, t) );
  }
  virtual Persistency::IO::MetaAlertAutoPtr metaAlertImpl(
                                        Persistency::MetaAlertPtrNN         ma,
                                        const Persistency::IO::Transaction &t)
  {
    ++called_[3];
    return Persistency::IO::MetaAlertAutoPtr( new IOMetaAlert(ma, t) );
  }

  int called_[4];
}; // struct TestIOConnection

} // unnamed namespace

#endif
