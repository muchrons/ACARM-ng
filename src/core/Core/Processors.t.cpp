/*
 * Processors.t.cpp
 *
 */
#include <tut/tut.hpp>
#include <string>

#include "ConfigIO/Singleton.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"
#include "Filter/Factory.hpp"
#include "Trigger/Factory.hpp"
#include "Core/Processors.hpp"
#include "TestHelpers/TestBase.hpp"
#include "TestHelpers/TimeoutChecker.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Core;
using namespace Core::Types::Proc;

namespace
{
struct TestClass: public TestHelpers::TestBase
{
  void config(const char *path) const
  {
    ConfigIO::Singleton::get()->rereadConfig(path);
  }

  Core::Types::SignedNodesFifo queue_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object            testObj;

factory tf("Core/Processors");
} // unnamed namespace


namespace tut
{

// test c-tor/d-tor
template<>
template<>
void testObj::test<1>(void)
{
  Processors p(queue_);
}


namespace
{
int filterCalls_=0;

struct TestFilterInterface: public Core::Types::Proc::Interface
{
  TestFilterInterface(void):
    Core::Types::Proc::Interface( TypeName("somefiltertype"), InstanceName("somefiltername"), EntryControlList::createDefaultAccept() )
  {
  }

  virtual void process(Node /*node*/, ChangedNodes &/*changedNodes*/)
  {
    ++filterCalls_;
  }

  virtual void heartbeat(unsigned int /*deadline*/)
  {
  }
}; // struct TestFilterInterface

class SomeTestFilterBuilder: public Filter::Factory::TFactoryBuilderBase
{
public:
  SomeTestFilterBuilder(void):
    name_("somefilter")
  {
  }

private:
  virtual FactoryPtr buildImpl(const Options &/*options*/) const
  {
    Core::Types::Proc::Processor::InterfaceAutoPtr ptr(new TestFilterInterface);
    return FactoryPtr(new Filter::InterfaceWrapper(ptr));
  }

  virtual const FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  const FactoryTypeName name_;
}; // class SomeTestFilterBuilder

const Commons::Factory::RegistratorHelper<Filter::Factory, SomeTestFilterBuilder> g_rhFilter;
} // unnamed namespace

namespace
{
int triggerCalls_=0;

struct TestTriggerInterface: public Core::Types::Proc::Interface
{
  TestTriggerInterface(void):
    Core::Types::Proc::Interface( TypeName("sometriggertype"), InstanceName("sometriggername"), EntryControlList::createDefaultAccept() )
  {
  }

  virtual void process(Node /*node*/, ChangedNodes &/*changedNodes*/)
  {
    ++triggerCalls_;
  }

  virtual void heartbeat(unsigned int /*deadline*/)
  {
  }
}; // struct TestInterface

class SomeTriggerBuilder: public Trigger::Factory::TFactoryBuilderBase
{
  public:
    SomeTriggerBuilder(void):
      name_("sometrigger")
  {
  }

  private:
    virtual FactoryPtr buildImpl(const Options &/*options*/) const
    {
      Core::Types::Proc::Processor::InterfaceAutoPtr ptr(new TestTriggerInterface);
      return FactoryPtr(new Trigger::InterfaceWrapper(ptr));
    }

    virtual const FactoryTypeName &getTypeNameImpl(void) const
    {
      return name_;
    }

    const FactoryTypeName name_;
}; // class SomeTriggerBuilder

const Commons::Factory::RegistratorHelper<Trigger::Factory, SomeTriggerBuilder> g_rhTrigger;
} // unnamed namespace

// test if both filters and triggers are used
template<>
template<>
void testObj::test<2>(void)
{
  // sanity check
  ensure("test filter not registered",  g_rhFilter.isRegistered() );
  ensure("test trigger not registered", g_rhTrigger.isRegistered() );
  // read dedicated config file
  config("testdata/filter_and_trigger.xml");

  // create processors
  Processors p(queue_);
  ensure_equals("filter has been called too fast",  filterCalls_,  0);
  ensure_equals("trigger has been called too fast", triggerCalls_, 0);

  // test example call
  queue_.push( Core::Types::SignedNode(TestHelpers::Persistency::makeNewNode(), TypeName("myT"), InstanceName("myN")) );
  p.process();  // this enqueues node in every filters

  // wait until everything's processed
  const TestHelpers::TimeoutChecker tc(16);
  while( ( filterCalls_<1 || triggerCalls_<1 )&& tc() )
    usleep(50*1000);

  // now check if every element has been processed
  ensure_equals("filter has not been called",  filterCalls_,  1);
  ensure_equals("trigger has not been called", triggerCalls_, 1);
}

} // namespace tut
