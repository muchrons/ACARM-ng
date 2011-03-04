/*
 * ProcessedSet.t.cpp
 *
 */

#include <tut.h>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/NewEvent/ProcessedSet.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"
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
    name_("entryname"),
    e_(name_, &bf_, &ts_)
  {
  }
  Persistency::IO::ConnectionPtrNN conn_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  TimeoutedSet                     ts_;
  ProcessedSet                     ps_;
  Entry::Name                      name_;
  Entry                            e_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/ProcessedSet");
} // unnamed namespace



namespace tut
{

// check if non-existing entry is reported as such
template<>
template<>
void testObj::test<1>(void)
{
  ensure("non-existing entry is reported as present", ps_.isProcessed( e_ )==false );
}

// check pruning
template<>
template<>
void testObj::test<2>(void)
{
  Entry e1("some entry", &bf_, &ts_);
  Entry e2("some other entry", &bf_, &ts_);
  ps_.markAsProcessed(e1, 1.0);
  ps_.markAsProcessed(e2, 2.0);
  ps_.prune();
  ensure("non-timeouted element has been pruned", ps_.isProcessed(e1) );
  ensure("non-timeouted element has been pruned", ps_.isProcessed(e2) );
  sleep(2);
  ps_.prune();
  ensure("timeouted element has been pruned", ps_.isProcessed(e1)==false );
  ensure("non-timeouted element has been pruned", ps_.isProcessed(e2) );
  sleep(1);
  ps_.prune();
  ensure("timeouted element has been pruned", ps_.isProcessed(e1)==false );
  ensure("non-timeouted element has been pruned", ps_.isProcessed(e2)==false );
}


// test if addition works
template<>
template<>
void testObj::test<3>(void)
{
  Entry e1("some entry", &bf_, &ts_);
  Entry e2("some other entry", &bf_, &ts_);
  ps_.markAsProcessed(e1, 1.0);
  ps_.markAsProcessed(e2, 2.0);
  ensure("element 1 not present", ps_.isProcessed(e1) );
  ensure("element 2 not present", ps_.isProcessed(e2) );
}

// test if after prune proper element is present in timeouted set
// TODO: this test fails
template<>
template<>
void testObj::test<4>(void)
{
  ps_.markAsProcessed(e_, 1.0);
  sleep(2);
  ensure("element timeouted after prune", ts_.isTimeouted(e_.getHash()) == false );
  ps_.prune();
  ensure("element not-timeouted after prune", ts_.isTimeouted(e_.getHash()));
}

} // namespace tut
