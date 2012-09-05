/*
 * CleanShutdownChecker.t.cpp
 *
 */
#include <tut/tut.hpp>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Core/CleanShutdownChecker.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/ConnectionUserStubBase.hpp"

using namespace Core;
using namespace Persistency::IO;

namespace
{

// data that must be preserved between testcases and used in factory (globaly)
struct GlobalConnectionData
{
  GlobalConnectionData(void):
    conn_( BackendFactory::create( "stubs", BackendFactory::Options() ) ),
    t_( conn_->createNewTransaction("gloabl_conn_for_clean_shutdown_check") )
  {
  }

  void reset(void)
  {
    dynamicConfigStub_=conn_->dynamicConfig("Clean Shutdown Checker", t_);
  }

  ConnectionPtrNN      conn_;
  Transaction          t_;
  DynamicConfigAutoPtr dynamicConfigStub_;
}; // struct GlobalConnectionData

// object's instance with reset() that will be called before each test case
GlobalConnectionData g_connectionData;


// helper class that forwards all calls to stubbed instance, that does not change
struct DynamicConfigForwarder: public Persistency::IO::DynamicConfig
{
public:
  DynamicConfigForwarder(const Owner &owner, Persistency::IO::Transaction &t, DynamicConfig &dc):
    Persistency::IO::DynamicConfig(owner, t),
    dc_(dc)
  {
    tut::ensure_equals("invalid owner for this module", owner.get(), std::string("Clean Shutdown Checker") );
  }

private:
  virtual void writeImpl(Persistency::IO::Transaction &/*t*/, const Key &key, const Value &value)
  {
    dc_.write(key, value);
  }
  virtual ValueNULL readImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    return dc_.read(key);
  }
  virtual Value readConstImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    return dc_.readConst(key);
  }

  virtual void removeImpl(Persistency::IO::Transaction &/*t*/, const Key &key)
  {
    dc_.remove(key);
  }

  virtual void iterateImpl(Persistency::IO::Transaction &/*t*/, IterationCallback &cb)
  {
    dc_.iterate(cb);
  }

  DynamicConfig &dc_;
}; // class DynamicConfig


// test connection that produces DynamicConfigForwarder as a DynamicConfig
struct CleanShutdownConnectionStub: public TestHelpers::Persistency::ConnectionUserStubBase
{
  virtual DynamicConfigAutoPtr dynamicConfigImpl(const DynamicConfig::Owner &owner,
                                                 Transaction                &t)
  {
    assert( g_connectionData.dynamicConfigStub_.get()!=NULL );
    return DynamicConfigAutoPtr( new DynamicConfigForwarder(owner, t, *g_connectionData.dynamicConfigStub_) );
  }
}; // struct CleanShutdownConnectionStub


// factory that creates CleanShutdownConnectionStub instances
struct TestConnectionBuilder: public Persistency::IO::BackendFactory::TFactoryBuilderBase
{
  TestConnectionBuilder(void):
    name_("cleanshutdownchecker_test_persistency")
  {
  }
  virtual FactoryPtr buildImpl(const Options &/*options*/) const
  {
    return TestConnectionBuilder::FactoryPtr(new CleanShutdownConnectionStub);
  }
  virtual const FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  const std::string  name_;
}; // struct TestConnectionBuilder

// element that registeres TestConnectionBuilder in the factory
const Commons::Factory::RegistratorHelper<Persistency::IO::BackendFactory, TestConnectionBuilder> g_rh;

// config that creates TestConnectionBuilder
const char *g_configContent=
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
  "<acarm_ng>"
  ""
  "  <general>"
  "    <url>http://test.url</url>"
  "    <cleanupInterval>42</cleanupInterval>"
  "    <cleanupOlder>41</cleanupOlder>"
  "    <pluginsDir>/some/dir/name</pluginsDir>"
  "    <alertsLimit>301</alertsLimit>"
  "  </general>"
  ""
  "  <persistency>"
  "    <type>cleanshutdownchecker_test_persistency</type>"
  "  </persistency>"
  ""
  "  <logger>"
  "    <appenders>"
  "      <File name=\"default\">"
  "        <output>acarm-ng.log</output>"
  "      </File>"
  "    </appenders>"
  ""
  "    <nodes appender=\"default\" threshold=\"debug\">"
  "    </nodes>"
  "  </logger>"
  ""
  "  <preprocessor/>"
  ""
  "  <filters/>"
  ""
  "  <triggers/>"
  ""
  "  <inputs/>"
  ""
  "</acarm_ng>"
;


struct TestClass: public TestHelpers::TestBase
{
  TestClass(void):
    TestHelpers::TestBase(g_configContent)  // set config as the current one
  {
    tut::ensure("test factory not registered", g_rh.isRegistered() );
    g_connectionData.reset();   // clear data before running each test case
  }
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/CleanShutdownChecker");
} // unnamed namespace


namespace tut
{

// test not-confirmed exit - smoke test
template<>
template<>
void testObj::test<1>(void)
{
  CleanShutdownChecker csc;
}

// test confirmed exit - smoke test
template<>
template<>
void testObj::test<2>(void)
{
  CleanShutdownChecker csc;
  csc.confirmCleanShutdown();
}

// test if last run was successful, when it was
template<>
template<>
void testObj::test<3>(void)
{
  {
    CleanShutdownChecker csc;
    csc.confirmCleanShutdown();
  }
  CleanShutdownChecker csc;
  ensure("last exit was not marked clean", csc.wasLastRunSuccessful() );
}

// test if last run was NOT successful, when it was not confirmed
template<>
template<>
void testObj::test<4>(void)
{
  {
    CleanShutdownChecker csc;
  }
  CleanShutdownChecker csc;
  ensure("last exit was marked clean", csc.wasLastRunSuccessful()==false );
}

// test if last run was NOT successful, when it was confirmed but not closed yet
template<>
template<>
void testObj::test<5>(void)
{
  CleanShutdownChecker csc1;
  csc1.confirmCleanShutdown();
  CleanShutdownChecker csc2;
  ensure("last exit was marked clean", csc2.wasLastRunSuccessful()==false );
}

} // namespace tut
