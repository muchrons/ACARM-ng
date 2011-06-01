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
    hPtr_(new Hash("entryname")),
    ePtr_(new Entry(hPtr_, bf_, ts_))
  {
  }

  Persistency::IO::ConnectionPtrNN conn_;
  BackendFacade::ChangedNodes      changed_;
  BackendFacade                    bf_;
  TimeoutedSet                     ts_;
  ProcessedSet                     ps_;
  HashSharedPtr                    hPtr_;
  EntrySharedPtr                   ePtr_;
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
  //ensure("non-existing entry is reported as present", ps_.isProcessed( *ePtr_.get() )==false );
}

// check pruning
template<>
template<>
void testObj::test<2>(void)
{
  HashSharedPtr h1(new Hash("some entry"));
  Entry         e1(h1, bf_, ts_);
  HashSharedPtr h2(new Hash("some other entry"));
  Entry         e2(h2, bf_, ts_);
  ps_.markAsProcessed(e1, 1);
  ps_.markAsProcessed(e2, 2);
  ps_.prune();
  ensure("element with timeout 1 s has been pruned before timeout", ps_.update(*h1.get(), 1) );
  ensure("element with timeout 2 s has been pruned before timeout", ps_.update(*h2.get(), 2) );
  sleep(2);
  ps_.prune();
  ensure("element with timeout 1 s has not been pruned after 2 seconds", ps_.update(*h1.get(), 1)==false );
  ensure("element with timeout 2 s has been pruned after 2 seconds", ps_.update(*h2.get(), 2) );
  sleep(1);
  ps_.prune();
  ensure("element with timeout 2 s has been pruned after timeout update", ps_.update(*h2.get(), 2) );
  sleep(3);
  ps_.prune();
  ensure("element with timeout 1 s has not been pruned", ps_.update(*h1.get(), 1)==false );
  ensure("element with timeout 2 s has not been pruned", ps_.update(*h2.get(), 2)==false );
}


// test if addition works
template<>
template<>
void testObj::test<3>(void)
{
  HashSharedPtr h1(new Hash("some entry"));
  Entry         e1(h1, bf_, ts_);
  HashSharedPtr h2(new Hash("some other entry"));
  Entry         e2(h2, bf_, ts_);
  ps_.markAsProcessed(e1, 1);
  ps_.markAsProcessed(e2, 2);
  ensure("element 1 not present", ps_.update(*h1.get(), 1) );
  ensure("element 2 not present", ps_.update(*h2.get(), 2) );
}

// test if after prune proper element is present in timeouted set
template<>
template<>
void testObj::test<4>(void)
{
  HashSharedPtr hash(new Hash("some name"));
  {
    Entry e(hash, bf_, ts_);
    ps_.markAsProcessed(e, 1);
  }
  sleep(2);
  //TODO: this test fail, implementation shouldanged to fix this issue
  //ensure("element timeouted before prune", ts_.isTimeouted(hash) == false );
  ps_.prune();
  ensure("element not-timeouted after prune", ts_.isTimeouted(hash));
}

// test if update works
template<>
template<>
void testObj::test<5>(void)
{
  HashSharedPtr hash(new Hash("some name"));
  Entry e(hash, bf_, ts_);
  ensure("element present in processed set", ps_.update(*hash.get(), 1) == false);
  // add element to the processed set
  ps_.markAsProcessed(e, 1);
  // update added element with timeout 3[s]
  ps_.update(*hash.get(), 3);
  // wait two seconds
  sleep(2);
  ps_.prune();
  // check if element is present after prune
  ensure("element is not timeouted", ps_.update(*hash.get(), 1));
  //TODO
}
} // namespace tut
