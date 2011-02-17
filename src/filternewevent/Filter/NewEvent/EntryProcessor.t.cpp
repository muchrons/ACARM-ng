/*
 * EntryProcessor.t.cpp
 *
 */

#include <tut.h>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/NewEvent/Strategy.hpp"
#include "Filter/NewEvent/EntryProcessor.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace Persistency;
using namespace Filter;
using namespace Filter::NewEvent;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    conn_( Persistency::IO::create() ),
    bf_(conn_, changed_, "testnewevent"),
    params_(2, 0.3),
    ep_(&bf_, &ps_, &ts_, params_)
  {
  }
  Persistency::IO::ConnectionPtrNN conn_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  ProcessedSet                     ps_;
  TimeoutedSet                     ts_;
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
  ensure_equals("priority not changed", pri1 + 0.3, pri2);
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
  ep_(leaf);
  const double   pri2=leaf->getMetaAlert()->getSeverityDelta();
  ensure_equals("priority not changed", pri1 + 0.3, pri2);
  ensure_equals("some node market as changed", changed_.size(), 1u);
}
} // namespace tut
