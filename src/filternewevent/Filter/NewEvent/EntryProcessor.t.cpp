/*
 * EntryProcessor.t.cpp
 *
 */

#include <tut.h>

#include "Persistency/IO/BackendFactory.hpp"
#include "System/Math/compareFP.hpp"
#include "Filter/NewEvent/Strategy.hpp"
#include "Filter/NewEvent/EntryProcessor.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;
using namespace Filter;
using namespace Filter::NewEvent;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    conn_( Persistency::IO::create() ),
    bf_(conn_, changed_, TypeName("testnewevent"), InstanceName("myname")),
    params_(2, 0.3),
    ep_(bf_, ps_, ts_, params_)
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  TimeoutedSet                     ts_;
  ProcessedSet                     ps_;
  NewEvent::Strategy::Parameters   params_;
  EntryProcessor                   ep_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/EntryProcessor");
} // unnamed namespace



namespace tut
{

// test name that is not on the list
template<>
template<>
void testObj::test<1>(void)
{
  GraphNodePtrNN leaf=makeNewLeaf(makeNewAlert("some alert"));
  const double   pri1=leaf->getMetaAlert()->getSeverityDelta();
  ep_(leaf);
  const double   pri2=leaf->getMetaAlert()->getSeverityDelta();
  ensure("priority not changed", System::Math::compareFP(pri1 + 0.3, pri2) );
  ensure_equals("some node market as changed", changed_.size(), 1u);
}

// test name on the list
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN leaf=makeNewLeaf(makeNewAlert("some alert"));
  const double   pri1=leaf->getMetaAlert()->getSeverityDelta();
  ep_(leaf);
  const double   pri2=leaf->getMetaAlert()->getSeverityDelta();
  ensure("priority not changed", System::Math::compareFP(pri1 + 0.3, pri2) );
  // TODO: reset changed_'s content
  ep_(leaf);
  const double   pri3=leaf->getMetaAlert()->getSeverityDelta();
  ensure("priority changed", System::Math::compareFP(pri2, pri3) );
  ensure_equals("some node market as changed", changed_.size(), 1u);
}

} // namespace tut
