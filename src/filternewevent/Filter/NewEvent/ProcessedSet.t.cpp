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
#include "TestHelpers/delay.hpp"

using namespace Persistency;
using namespace Core::Types::Proc;
using namespace Filter;
using namespace Filter::NewEvent;
using namespace TestHelpers::Persistency;
using TestHelpers::delay;

namespace
{

struct TestClass: private TestStubs
{

  TestClass(void):
    conn_( Persistency::IO::create() ),
    bf_(conn_, changed_, TypeName("testnewevent"), InstanceName("myname")),
    h1_("entryname"),
    h2_("otherentryname"),
    e1Ptr_(new Entry(h1_, bf_, ts_)),
    e2Ptr_(new Entry(h2_, bf_, ts_))
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  TimeoutedSet                     ts_;
  ProcessedSet                     ps_;
  Hash                             h1_;
  Hash                             h2_;
  EntrySharedPtr                   e1Ptr_;
  EntrySharedPtr                   e2Ptr_;

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
  ensure("non-existing entry is reported as present", ps_.update( h1_, 1 ) == false );
}

// check pruning
template<>
template<>
void testObj::test<2>(void)
{
  ps_.markAsProcessed(e1Ptr_, 1);
  ps_.markAsProcessed(e2Ptr_, 2);
  ps_.prune();
  ensure("element with timeout 1 s has been pruned before timeout", ps_.update(h1_, 1) );
  ensure("element with timeout 2 s has been pruned before timeout", ps_.update(h2_, 3) );
  delay(2);
  ps_.prune();
  ensure("element with timeout 1 s has not been pruned after 2 seconds", ps_.update(h1_, 1)==false );
  ensure("element with timeout 2 s has been pruned after 2 seconds", ps_.update(h2_, 2) );
  delay(1);
  ps_.prune();
  ensure("element with timeout 2 s has been pruned after timeout update", ps_.update(h2_, 1) );
  delay(2);
  ps_.prune();
  ensure("element with timeout 1 s has not been pruned", ps_.update(h1_, 1)==false );
  ensure("element with timeout 2 s has not been pruned", ps_.update(h2_, 2)==false );
}


// test if addition works
template<>
template<>
void testObj::test<3>(void)
{
  ps_.markAsProcessed(e1Ptr_, 1);
  ps_.markAsProcessed(e2Ptr_, 2);
  ensure("element 1 not present", ps_.update(h1_, 1) );
  ensure("element 2 not present", ps_.update(h2_, 2) );
}

// test if after prune proper element is present in timeouted set
template<>
template<>
void testObj::test<4>(void)
{
  {
    EntrySharedPtr e(new Entry(h1_, bf_, ts_));
    ps_.markAsProcessed(e, 1);
  }
  delay(2);
  ensure("element timeouted before prune", ts_.isTimeouted(h1_) == false );
  ps_.prune();
  ensure("element not-timeouted after prune", ts_.isTimeouted(h1_));
}

// test if update works
template<>
template<>
void testObj::test<5>(void)
{
  Hash hash("some name");
  EntrySharedPtr e(new Entry(hash, bf_, ts_));
  ensure("element present in processed set", ps_.update(hash, 1) == false);
  // add element to the processed set
  ps_.markAsProcessed(e, 1);
  // update added element with timeout 3[s]
  ps_.update(hash, 3);
  ps_.prune();
  // check if element is present after prune
  ensure("element is not timeouted", ps_.update(hash, 1));
}

} // namespace tut
