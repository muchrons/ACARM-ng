/*
 * Factory.t.cpp
 *
 */
#include <tut.h>
#include <cstring>
#include <memory>
#include <cassert>

#include "Filter/Factory.hpp"
#include "ConfigIO/Singleton.hpp"
#include "Commons/Factory/RegistratorHelper.hpp"

using namespace std;
using namespace Filter;
using namespace Core::Types::Proc;

namespace
{

struct TestClass
{
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
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/Factory");
} // unnamed namespace


namespace tut
{

// test creating empty set of filters
template<>
template<>
void testObj::test<1>(void)
{
  const FiltersCollection fc=create(queue_);
  ensure_equals("some elements returend for empty filter's list", fc.size(), 0u);
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
    Interface( "narf", "narf", EntryControlList::createDefaultAccept() )
  {
  }

  virtual void process(Node /*node*/, ChangedNodes &/*changedNodes*/)
  {
  }
}; // struct TestInterface

class SomeBuilder: public Factory::TFactoryBuilderBase
{
public:
  SomeBuilder(void):
    name_("somefilter")
  {
  }

private:
  virtual FactoryPtr buildImpl(const Options &options) const
  {
    Options::const_iterator it=options.begin();
    ensure("empty collection", it!=options.end() );

    ensure_equals("invalid option 1 key",   it->first,  "opt1");
    ensure_equals("invalid option 1 value", it->second, "kszy");
    ++it;

    ensure("just one element in collection", it!=options.end() );
    ensure_equals("invalid option 2 key",   it->first,  "opt2");
    ensure_equals("invalid option 2 value", it->second, "narf");
    ++it;

    ensure("too many elements in collection", it==options.end() );

    return Processor::InterfaceAutoPtr(new TestInterface);
  }

  virtual const FactoryTypeName &getTypeNameImpl(void) const
  {
    return name_;
  }

  const FactoryTypeName name_;
}; // class SomeBuilder

const Commons::Factory::RegistratorHelper<Factory, SomeBuilder> g_rh;

} // unnamed namespace

// test creating some exising filter
template<>
template<>
void testObj::test<3>(void)
{
  assert( g_rh.isRegistered() && "oops - registration failed" );
  ConfigIO::Singleton::get()->rereadConfig("testdata/some_filter.xml");
  const FiltersCollection fc=create(queue_);
  ensure_equals("no filters created", fc.size(), 1u);
}

} // namespace tut
