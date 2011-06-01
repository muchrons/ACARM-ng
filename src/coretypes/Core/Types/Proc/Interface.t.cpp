/*
 * Interface.t.cpp
 *
 */
#include <tut.h>
#include <boost/scoped_ptr.hpp>
#include <cassert>

#include "Core/Types/Proc/Interface.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace Core::Types::Proc;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestInterface: public Interface
{
  explicit TestInterface(bool *dtor):
    Interface( TypeName("testinterfacetype"), InstanceName("testinterfacename"), EntryControlList::createDefaultAccept() ),
    calls_(0),
    heartbeats_(0),
    dtor_(dtor)
  {
    assert(dtor_!=NULL);
  }

  virtual ~TestInterface(void)
  {
    *dtor_=true;
  }

  virtual void process(Node, ChangedNodes&)
  {
    ++calls_;
  }

  virtual void heartbeat(unsigned int)
  {
    ++heartbeats_;
  }


  int   calls_;
  int   heartbeats_;
  bool *dtor_;
};


struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    dtor_(false),
    ptr_( new TestInterface(&dtor_) ),
    ti_(ptr_)
  {
  }

  bool                          dtor_;
  TestInterface                *ptr_;
  boost::scoped_ptr<Interface>  ti_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/Interface");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name set", ti_->getName().str(), "testinterfacename");
}

// test getting type
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("invalid type set", ti_->getType().str(), "testinterfacetype");
}
// test if d-tor is virtual
template<>
template<>
void testObj::test<3>(void)
{
  ensure("pre failed", dtor_==false);
  ti_.reset();
  ensure("d-tor is not virtual", dtor_==true);
}

// test if process() is virtual
template<>
template<>
void testObj::test<4>(void)
{
  ensure_equals("pre-condition failed", ptr_->calls_, 0);
  Interface::ChangedNodes changed;
  ti_->process( makeNewLeaf(), changed );
  ensure_equals("process() is not virtual", ptr_->calls_, 1);
}

// test if heartbeat() is virtual call
template<>
template<>
void testObj::test<5>(void)
{
  ensure_equals("pre-condition failed", ptr_->heartbeats_, 0);
  ti_->heartbeat(42);
  ensure_equals("heartbeat not send", ptr_->heartbeats_, 1);
}

} // namespace tut
