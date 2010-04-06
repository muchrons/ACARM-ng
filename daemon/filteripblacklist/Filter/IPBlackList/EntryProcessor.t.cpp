/*
 * EntryProcessor.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/IPBlackList/EntryProcessor.hpp"
#include "Filter/IPBlackList/DShieldParser.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter;
using namespace Filter::IPBlackList;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    conn_( Persistency::IO::create() ),
    dsp_("084.109.175.233 687     352     2009-12-31      2010-03-12"),
    bl_( dsp_.begin(), dsp_.end() ),
    bf_(conn_, changed_, "testdipblacklist"),
    ep_(&bl_, &bf_, 0.3)
  {
    assert( changed_.size()==0 );
  }

  Persistency::IO::ConnectionPtrNN conn_;
  DShieldParser                    dsp_;
  BlackList                        bl_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  EntryProcessor                   ep_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/IPBlackList/EntryProcessor");
} // unnamed namespace


namespace tut
{

// test host that is not on the list
template<>
template<>
void testObj::test<1>(void)
{
  GraphNodePtrNN leaf=makeNewLeaf("1.2.3.4", "84.109.175.233");
  const double   pri1=leaf->getMetaAlert()->getSeverityDelta();
  ep_(leaf);
  const double   pri2=leaf->getMetaAlert()->getSeverityDelta();
  ensure_equals("priority changed", pri1, pri2);
  ensure_equals("some node marked as changed", changed_.size(), 0u);
}

// test host on the list
template<>
template<>
void testObj::test<2>(void)
{
  GraphNodePtrNN leaf=makeNewLeaf("84.109.175.233", "1.2.3.5");
  const double   pri1=leaf->getMetaAlert()->getSeverityDelta();
  ep_(leaf);
  const double   pri2=leaf->getMetaAlert()->getSeverityDelta();
  ensure_equals("priority changed", pri1+0.3, pri2);
  ensure_equals("some node marked as changed", changed_.size(), 1u);
}

} // namespace tut
