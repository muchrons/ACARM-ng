/*
 * InterfaceImpl.t.cpp
 *
 */
#include <tut.h>

#include "Core/Types/Proc/InterfaceImpl.hpp"
#include "Persistency/Stubs/TestHelpers.hpp"

using namespace Core::Types::Proc;
using namespace Persistency;
using namespace Persistency::Stubs;

namespace
{

struct TestStrategy
{
  TestStrategy(void):
    calls_(0)
  {
  }

  virtual void process(GraphNodePtrNN, Interface::ChangedNodes&)
  {
    ++calls_;
  }

  int calls_;
};


struct TestClass
{
  TestClass(void):
    impl_("somename", strat_)
  {
  }

  TestStrategy                 strat_;
  InterfaceImpl<TestStrategy&> impl_;
};

typedef TestClass TestClass;
typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Core/Types/Proc/InterfaceImpl");
} // unnamed namespace


namespace tut
{

// test getting name
template<>
template<>
void testObj::test<1>(void)
{
  ensure_equals("invalid name set", impl_.getName(), "somename");
}

// test passing call to process
template<>
template<>
void testObj::test<2>(void)
{
  ensure_equals("pre-condition failed", strat_.calls_, 0);
  Interface::ChangedNodes changed;
  impl_.process( makeNewLeaf(), changed );
  ensure_equals("process() is not virtual", strat_.calls_, 1);
}

} // namespace tut
