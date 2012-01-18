/*
 * Factory.t.cpp
 *
 */
#include <tut.h>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <cassert>

#include "Trigger/Factory.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/TimeoutChecker.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Trigger;
using namespace Core::Types::Proc;

namespace
{

struct TestClass: public TestHelpers::TestBase
{
  TestClass(void):
    sn_( TestHelpers::Persistency::makeNewLeaf(), TypeName("mytesttype"), InstanceName("mytestinstance") )
  {
  }

  ~TestClass(void)
  {
    try
    {
      // clean-up
      ConfigIO::Singleton::get()->rereadConfig();
    }
    catch(...)
    {
      assert(!"oops - unexpected problem in test d-tor");
    }
  }

  Core::Types::SignedNodesFifo queue_;
  Core::Types::SignedNode      sn_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Trigger/Factory");
} // unnamed namespace


namespace tut
{

// test creating empty set of triggers
template<>
template<>
void testObj::test<1>(void)
{
  const TriggersCollection fc=create(queue_);
  ensure_equals("some elements returend for empty triggers's list", fc.size(), 0u);
}

// test exception when requesting non-existing builder
template<>
template<>
void testObj::test<2>(void)
{
  ConfigIO::Singleton::get()->rereadConfig("testdata/non_existing_builder.xml");
  try
  {
    create(queue_);
    fail("create() didn't throw when no factory is registered");
  }
  catch(const Commons::Factory::ExceptionBuilderDoesNotExist&)
  {
    // this is expected
  }
}


namespace
{

struct TestInterface: public Interface
{
  TestInterface(void):
    Interface( TypeName("narf"), InstanceName("narf"), EntryControlList::createDefaultAccept() )
  {
  }

  virtual void process(Node /*node*/, ChangedNodes &/*changedNodes*/)
  {
  }

  virtual void heartbeat(unsigned int /*deadline*/)
  {
  }
}; // struct TestInterface

class SomeBuilder: public Factory::TFactoryBuilderBase
{
public:
  SomeBuilder(void):
    name_("sometrigger")
  {
  }

private:
  virtual FactoryPtr buildImpl(const Options &options) const
  {
    Options::const_iterator it=options.begin();

    ensure("empty collection", it!=options.end() );
    ensure_equals("invalid option 1 key",   it->first,  "name");
    ensure_equals("invalid option 1 value", it->second, "triggername");
    ++it;
    //a first and a third block but not the second. Iterator is moved after first block but there are still 1s in the text.
    ensure("just one element in collection", it!=options.end() );
    ensure_equals("invalid option 2 key",   it->first,  "opt1");
    ensure_equals("invalid option 2 value", it->second, "kszy");
    ++it;

    ensure("not enought elements in collection", it!=options.end() );
    ensure_equals("invalid option 3 key",   it->first,  "opt2");
    ensure_equals("invalid option 3 value", it->second, "narf");
    ++it;

    ensure("too many elements in collection", it==options.end() );

    InterfaceWrapper::InterfaceAutoPtr ptr(new TestInterface);
    return FactoryPtr(new InterfaceWrapper(ptr));
  }

  virtual const FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  const FactoryTypeName name_;
}; // class SomeBuilder

const Commons::Factory::RegistratorHelper<Factory, SomeBuilder> g_rh;

} // unnamed namespace

// test creating some exising trigger
template<>
template<>
void testObj::test<3>(void)
{
  ensure("oops - registration failed", g_rh.isRegistered());
  ConfigIO::Singleton::get()->rereadConfig("testdata/some_trigger.xml");
  const TriggersCollection fc=create(queue_);
  ensure_equals("no triggers created", fc.size(), 1u);
}


namespace
{
// global counter to make things easier for testing
int g_testInterfaceCalled=-1;

// test interface to be returned
struct PPTestInterface: public Interface
{
  PPTestInterface(void):
    Interface( TypeName("preproc_test_trigger"), InstanceName("narf"), EntryControlList::createDefaultAccept() )
  {
    g_testInterfaceCalled=0;
  }
  virtual void process(Node /*node*/, ChangedNodes &/*changedNodes*/)
  {
    ++g_testInterfaceCalled;
  }
  virtual void heartbeat(unsigned int /*deadline*/)
  {
  }
}; // struct PPTestInterface

// test builder for given interface
class SomePPBuilder: public Factory::TFactoryBuilderBase
{
public:
  SomePPBuilder(void):
    name_("preproc_test_trigger")
  {
  }

private:
  virtual FactoryPtr buildImpl(const Options &/*options*/) const
  {
    InterfaceWrapper::InterfaceAutoPtr ptr(new PPTestInterface);
    return FactoryPtr(new InterfaceWrapper(ptr));
  }
  virtual const FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  const FactoryTypeName name_;
}; // class SomePPBuilder

const Commons::Factory::RegistratorHelper<Factory, SomePPBuilder> g_rhPP;
} // unnamed namespace

// check if trigger will not be called when preprocessor will not accept it
template<>
template<>
void testObj::test<4>(void)
{
  ensure("oops - registration failed", g_rhPP.isRegistered());
  readConfigFile("testdata/preproc_test_trigger_reject_all.xml");
  TriggersCollection fc=create(queue_);
  ensure_equals("no triggers created", fc.size(), 1u);
  ensure_equals("interface already called", g_testInterfaceCalled, 0);
  fc.at(0)->process(sn_);
  usleep(500*1000);         // wait a while, until data is processed...
  ensure_equals("call not blocked by preprocessor", g_testInterfaceCalled, 0);
}

// check if trigger will be called when preprocessor will accept it
template<>
template<>
void testObj::test<5>(void)
{
  ensure("oops - registration failed", g_rhPP.isRegistered());
  readConfigFile("testdata/preproc_test_trigger_accept_all.xml");
  TriggersCollection fc=create(queue_);
  ensure_equals("no triggers created", fc.size(), 1u);
  ensure_equals("interface already called", g_testInterfaceCalled, 0);
  fc.at(0)->process(sn_);
  // wait a while, until data is processed...
  const TestHelpers::TimeoutChecker tc(2);
  while( g_testInterfaceCalled!=1 && tc() )
  { /* wait */ }
  usleep(20*1000);
  ensure_equals("call blocked by preprocessor", g_testInterfaceCalled, 1);
}

} // namespace tut
