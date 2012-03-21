/*
 * IOStubs.t.hpp
 *
 */
#ifndef INCLUDE_IOSTUBS_T_HPP_FILE
#define INCLUDE_IOSTUBS_T_HPP_FILE

#include <tut.h>
#include <string>

#include "Commons/Convert.hpp"
#include "Persistency/GraphNode.hpp"
#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/TestTransactionAPI.t.hpp"
#include "Persistency/IO/IOStubs.t.hpp"
#include "Persistency/TestHelpers.t.hpp"

namespace
{

class IOAlert: public Persistency::IO::Alert
{
public:
  IOAlert(Persistency::AlertPtrNN       alert,
          Persistency::IO::Transaction &t):
    Persistency::IO::Alert(alert, t),
    alert_(alert),
    calls_(0)
  {
  }

  virtual void saveImpl(Persistency::IO::Transaction&)
  {
    ++calls_;
    tut::ensure("invalid object", get().get()==alert_.get() );
  }

  Persistency::AlertPtrNN alert_;
  int                     calls_;
}; // class IOAlert


class IOHost: public Persistency::IO::Host
{
public:
  IOHost(Persistency::HostPtrNN        host,
         Persistency::IO::Transaction &t):
    Persistency::IO::Host(host, t),
    host_(host)
  {
  }

  virtual void setNameImpl(Persistency::IO::Transaction &,
                           const Persistency::Host::Name &/*name*/)
  {
    ++calls_;
    tut::ensure("invalid host", get().get()==host_.get() );
  }

  Persistency::HostPtrNN host_;
  int                    calls_;
}; // class IOHost

class IOMetaAlert: public Persistency::IO::MetaAlert
{
public:
  IOMetaAlert(Persistency::MetaAlertPtrNN   ma,
              Persistency::IO::Transaction &t):
    Persistency::IO::MetaAlert(ma, t),
    ma_(ma)
  {
    tut::ensure("invalid meta alert", get().get()==ma_.get() );

    for(unsigned int i=0; i<sizeof(called_)/sizeof(called_[0]); ++i)
      called_[i]=0;
  }

  virtual void saveImpl(Persistency::IO::Transaction &)
  {
    ++called_[0];
  }
  virtual void markAsTriggeredImpl(Persistency::IO::Transaction &, const std::string &)
  {
    ++called_[7];
  }
  virtual void markAsUsedImpl(Persistency::IO::Transaction &)
  {
    ++called_[1];
  }
  virtual void markAsUnusedImpl(Persistency::IO::Transaction &)
  {
    ++called_[2];
  }
  virtual void updateSeverityDeltaImpl(Persistency::IO::Transaction &, double /*delta*/)
  {
    ++called_[3];
  }
  virtual void updateCertaintyDeltaImpl(Persistency::IO::Transaction &, double /*delta*/)
  {
    ++called_[4];
  }
  virtual void addChildImpl(Persistency::IO::Transaction &, Persistency::MetaAlertPtrNN /*child*/)
  {
    ++called_[5];
  }
  virtual void associateWithAlertImpl(Persistency::IO::Transaction &, Persistency::AlertPtrNN /*alert*/)
  {
    ++called_[6];
  }

  Persistency::MetaAlertPtrNN ma_;
  int                         called_[8];
}; // class MetaAlert


struct IODynamicConfig: public Persistency::IO::DynamicConfig
{
public:
  IODynamicConfig(const Owner &owner, Persistency::IO::Transaction &t):
    Persistency::IO::DynamicConfig(owner, t)
  {
    memset(calls_, 0, sizeof(calls_));
  }

  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/, const Value &/*value*/)
  {
    ++calls_[0];
  }

  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    ++calls_[1];
    return ValueNULL("alice has a wonderland");
  }

  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    ++calls_[2];
    return Value("i'm const");
  }

  virtual void removeImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    ++calls_[3];
  }

  virtual void iterateImpl(Persistency::IO::Transaction &/*t*/, IterationCallback &/*cb*/)
  {
    ++calls_[4];
  }

  int calls_[5];
}; // class IODynamicConfig


struct IODynamicConfigCounter: public Persistency::IO::DynamicConfig
{
public:
  IODynamicConfigCounter(Persistency::IO::Transaction &t, const std::string &keyValue):
    Persistency::IO::DynamicConfig("some owner", t),
    keyValue_(keyValue),
    isNull_(true),
    id_(1234567890)
  {
  }

  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    tut::ensure_equals("invalid key to be written", key.get(), keyValue_);
    typedef Persistency::GraphNode::ID::Numeric NumericID;
    const NumericID newID=Commons::Convert::to<NumericID>( value.get() );
    if(isNull_)
      tut::ensure_equals("invalid initial value to be written", newID, 1u);
    else
      tut::ensure_equals("invalid new ID to be written", newID, id_+1);
    id_    =newID;
    isNull_=false;
  }

  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    tut::ensure_equals("invalid key to be read", keyValue_, key.get() );
    if(isNull_)
      return ValueNULL();
    return ValueNULL( Commons::Convert::to<std::string>(id_) );
  }

  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    tut::fail("readConst() should NOT be called at all");
    return Value("???");
  }

  virtual void removeImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    tut::fail("remove() should not be called here");
  }

  virtual void iterateImpl(Persistency::IO::Transaction &/*t*/, IterationCallback &/*cb*/)
  {
    tut::fail("iterate() should not be called here");
  }

  const std::string                   keyValue_;
  bool                                isNull_;
  Persistency::GraphNode::ID::Numeric id_;
}; // class IODynamicConfigCounter


struct IODynamicConfigMap: public Persistency::IO::DynamicConfig
{
public:
  typedef std::map<std::string, std::string> Memory;

  IODynamicConfigMap(Persistency::IO::Transaction &t, Memory &mem):
    Persistency::IO::DynamicConfig("some owner", t),
    mem_(mem)
  {
  }

  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    mem_[ key.get() ]=value.get();
  }

  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    if( mem_.find( key.get() )==mem_.end() )
      return ValueNULL();
    return ValueNULL( mem_[ key.get() ] );
  }

  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &/*key*/)
  {
    tut::fail("readConst() should NOT be called at all");
    return Value("???");
  }

  virtual void removeImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    mem_.erase( key.get() );
  }

  virtual void iterateImpl(Persistency::IO::Transaction &/*t*/, IterationCallback &cb)
  {
    for(Memory::const_iterator it=mem_.begin(); it!=mem_.end(); ++it)
      if( cb.process(it->first, it->second)==false )
        break;
  }

  Memory &mem_;
}; // class IODynamicConfigMap


class IORestorer: public Persistency::IO::Restorer
{
public:
  IORestorer(Persistency::IO::Transaction &t):
    Persistency::IO::Restorer(t),
    restoreAll_(0),
    restoreBetween_(0)
  {
  }

  virtual void restoreAllInUseImpl(Persistency::IO::Transaction&, NodesVector&)
  {
    ++restoreAll_;
  }

  virtual void restoreBetweenImpl(Persistency::IO::Transaction&,
      NodesVector&,
      const Persistency::Timestamp&,
      const Persistency::Timestamp&)
  {
    ++restoreBetween_;
  }

  int restoreAll_;
  int restoreBetween_;
}; // class IORestorer


class IOHeartbeats: public Persistency::IO::Heartbeats
{
public:
  IOHeartbeats(const Owner &owner, Persistency::IO::Transaction &t):
    Persistency::IO::Heartbeats(owner, t),
    report_(0)
  {
  }

  virtual void reportImpl(Persistency::IO::Transaction &,
                          const Owner &,
                          const Module &,
                          Persistency::Timestamp,
                          unsigned int)
  {
    ++report_;
  }

  int report_;
}; // class IOHeartbeats



struct TestIOConnection: public Persistency::IO::Connection
{
  TestIOConnection(void)
  {
    for(unsigned int i=0; i<sizeof(called_)/sizeof(called_[0]); ++i)
      called_[i]=0;
  }

  virtual Persistency::IO::TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                                          const std::string    &name)
  {
    ++called_[0];
    return Persistency::IO::TransactionAPIAutoPtr( new TestTransactionAPI(mutex, name) );
  }
  virtual Persistency::IO::AlertAutoPtr alertImpl(Persistency::AlertPtrNN       alert,
                                                  Persistency::IO::Transaction &t)
  {
    ++called_[1];
    return Persistency::IO::AlertAutoPtr( new IOAlert(alert, t) );
  }
  virtual Persistency::IO::HostAutoPtr hostImpl(Persistency::HostPtrNN        host,
                                                Persistency::IO::Transaction &t)
  {
    ++called_[2];
    return Persistency::IO::HostAutoPtr( new IOHost(host, t) );
  }
  virtual Persistency::IO::MetaAlertAutoPtr metaAlertImpl(Persistency::MetaAlertPtrNN   ma,
                                                          Persistency::IO::Transaction &t)
  {
    ++called_[3];
    return Persistency::IO::MetaAlertAutoPtr( new IOMetaAlert(ma, t) );
  }

  virtual Persistency::IO::DynamicConfigAutoPtr dynamicConfigImpl(const Persistency::IO::DynamicConfig::Owner &owner,
                                                                  Persistency::IO::Transaction                &t)
  {
    ++called_[6];
    return Persistency::IO::DynamicConfigAutoPtr( new IODynamicConfig(owner, t) );
  }

  virtual Persistency::IO::RestorerAutoPtr restorerImpl(Persistency::IO::Transaction &t)
  {
    ++called_[4];
    return Persistency::IO::RestorerAutoPtr( new IORestorer(t) );
  }

  virtual size_t removeEntriesOlderThanImpl(size_t /*days*/, Persistency::IO::Transaction &/*t*/)
  {
    ++called_[5];
    return 42;
  }

  virtual void issuePeriodicSystemQueriesImpl(Persistency::IO::Transaction &/*t*/)
  {
    ++called_[8];
  }

  virtual void issuePeriodicUserQueriesImpl(Persistency::IO::Transaction &/*t*/)
  {
    ++called_[9];
  }

  virtual Persistency::IO::HeartbeatsAutoPtr heartbeatsImpl(const Persistency::IO::Heartbeats::Owner &owner,
                                                            Persistency::IO::Transaction             &t)
  {
    ++called_[7];
    return Persistency::IO::HeartbeatsAutoPtr( new IOHeartbeats(owner, t) );
  }


  int called_[10];
}; // struct TestIOConnection

} // unnamed namespace

#endif
