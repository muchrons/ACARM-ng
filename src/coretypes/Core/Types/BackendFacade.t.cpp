/*
 * BackendFacade.t.cpp
 *
 */
#include <tut.h>
#include <memory>
#include <vector>
#include <string>
#include <cstring>
#include <cassert>
#include <boost/tuple/tuple.hpp>

#include "Core/Types/BackendFacade.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"

using namespace std;
using namespace Persistency;
using namespace Core::Types;
using namespace Core::Types::Proc;

namespace
{

struct TestProxy: public BackendFacade
{
  explicit TestProxy(IO::ConnectionPtrNN conn):
    BackendFacade(conn, CategoryName("evilinput"), TypeName("sometest"), InstanceName("myinstance") )
  {
  }

  Persistency::IO::ConnectionPtrNN getConnectionPublic(void)
  {
    return getConnection();
  }

  const TypeName &getTypePublic(void) const
  {
    return getType();
  }

  const InstanceName &getNamePublic(void) const
  {
    return getName();
  }

  void doSth(void)
  {
    beginTransaction();
  }
}; // struct TestProxy


typedef boost::tuple<IO::Heartbeats::Owner, IO::Heartbeats::Module, unsigned int> HbEntry;
typedef std::vector<HbEntry>                                                      HbLog;

struct TestHeartbeats: public IO::Heartbeats
{
  TestHeartbeats(const Owner &o, IO::Transaction &t, HbLog &log):
    IO::Heartbeats(o, t),
    log_(&log)
  {
  }

private:
  virtual void reportImpl(IO::Transaction &/*t*/, const Owner &o, const Module &m, Timestamp /*reported*/, unsigned int timeout)
  {
    const HbEntry e(o, m, timeout);
    assert(log_!=NULL);
    log_->push_back(e);
  }

  HbLog *log_;
}; // struct TestHeartbeats


struct ConnHeartbeatHolder: public TestHelpers::Persistency::ConnectionUserStubBase
{
  explicit ConnHeartbeatHolder(HbLog &log):
    log_(&log)
  {
  }

  virtual IO::HeartbeatsAutoPtr heartbeatsImpl(const IO::Heartbeats::Owner &owner,
                                                     IO::Transaction       &t)
  {
    assert(log_!=NULL);
    IO::HeartbeatsAutoPtr ptr(new TestHeartbeats(owner, t, *log_));
    return ptr;
  }

  HbLog *log_;
}; // struct ConnHeartbeatHolder


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    bf_( new TestProxy( IO::ConnectionPtrNN(IO::create()) ) ),
    bfHb_( IO::ConnectionPtrNN(new ConnHeartbeatHolder(hbLog_)) )
  {
    assert( bf_.get()!=NULL );
  }

  TestHelpers::Persistency::TestStubs cfg_;
  boost::scoped_ptr<TestProxy>        bf_;
  HbLog                               hbLog_;
  TestProxy                           bfHb_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/BackendFacade");
} // unnamed namespace


namespace tut
{

// check if d-tor, when nothing's done, works fine
template<>
template<>
void testObj::test<1>(void)
{
  bf_.reset();
}

// test commiting empty change-set
template<>
template<>
void testObj::test<2>(void)
{
  bf_->commitChanges();
}

// test commiting some change
template<>
template<>
void testObj::test<3>(void)
{
  bf_->doSth();
  bf_->commitChanges();
}

// test getting connection
template<>
template<>
void testObj::test<4>(void)
{
  ensure("NULL connection received", bf_->getConnectionPublic().get()!=NULL);
}

// test getting proxy's processor name
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("invalid name", bf_->getNamePublic().str(), "myinstance");
}

// test if it is possible to reopn new transaction after commiting changes
template<>
template<>
void testObj::test<6>(void)
{
  // transaction 1
  bf_->doSth();
  bf_->commitChanges();
  // transaction 2
  bf_->doSth();
  bf_->commitChanges();
}


namespace
{
struct TestCustomActionRun: public BackendFacade::CustomIOInterface
{
  virtual void customAction(Persistency::IO::ConnectionPtrNN /*conn*/, Persistency::IO::Transaction &/*t*/)
  {
    ++count_;
  }

  int count_;
};
} // unnamed namespace

// test performing custom IO operations
template<>
template<>
void testObj::test<7>(void)
{
  TestCustomActionRun tmp;
  tmp.count_=0;
  bf_->performCustomIO(tmp);
  ensure_equals("invalid number of calls", tmp.count_, 1);
}


namespace
{
struct TestCustomActionDtor: public BackendFacade::CustomIOInterface
{
  explicit TestCustomActionDtor(bool &isRun):
    isRun_(isRun)
  {
    isRun_=false;
  }

  ~TestCustomActionDtor(void)
  {
    isRun_=true;
  }

  virtual void customAction(Persistency::IO::ConnectionPtrNN /*conn*/, Persistency::IO::Transaction &/*t*/)
  {
  }

private:
  bool &isRun_;
};
} // unnamed namespace

// test if d-tor of TestCustomActionRun is virtual
template<>
template<>
void testObj::test<8>(void)
{
  bool isRun=false;
  {
    boost::scoped_ptr<BackendFacade::CustomIOInterface> tmp(new TestCustomActionDtor(isRun) );
    ensure("bug in test", isRun==false);
  }
  ensure("d-tor not run", isRun);
}

// test getting proxy's processor type
template<>
template<>
void testObj::test<9>(void)
{
  ensure_equals("invalid type", bf_->getTypePublic().str(), "sometest");
}

// test sending heartbeat for the component
template<>
template<>
void testObj::test<10>(void)
{
  bfHb_.heartbeat(42);
  ensure_equals("invalid number of entries", hbLog_.size(), 1u);
  ensure_equals("invalid owner", hbLog_[0].get<0>().get(), string("evilinput::sometest/myinstance"));
  ensure_equals("invalid module", hbLog_[0].get<1>().get(), string("self"));
  ensure_equals("invalid timeout", hbLog_[0].get<2>(), 42u);
}

// test sending heartbeat for the module of the component
template<>
template<>
void testObj::test<11>(void)
{
  bfHb_.heartbeat("blackbetty", 42);
  ensure_equals("invalid number of entries", hbLog_.size(), 1u);
  ensure_equals("invalid owner", hbLog_[0].get<0>().get(), string("evilinput::sometest/myinstance"));
  ensure_equals("invalid module", hbLog_[0].get<1>().get(), string("blackbetty"));
  ensure_equals("invalid timeout", hbLog_[0].get<2>(), 42u);
}

// TODO: test createDynamicConfig method

} // namespace tut
