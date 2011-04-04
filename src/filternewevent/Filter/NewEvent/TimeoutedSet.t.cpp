/*
 * TimeoutedSet.t.cpp
 *
 */
#include <tut.h>

#include "Filter/NewEvent/Entry.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Filter/NewEvent/TestConnection.t.hpp"

using namespace std;
using namespace Filter;
using namespace Filter::NewEvent;
using namespace TestHelpers::Persistency;

namespace
{
  struct TestClass
  {

    TestClass(void):
      tconn_(new TestConnection),
      conn_( tconn_ ),
      bf_(conn_, changed_, "testnewevent"),
      owner_("Filter::NewEvent")
    {
    }

    TestConnection                        *tconn_;
    Persistency::IO::ConnectionPtrNN      conn_;
    BackendFacade::ChangedNodes           changed_;
    BackendFacade                         bf_;
    TimeoutedSet                          ts_;
    Persistency::IO::DynamicConfig::Owner owner_;
  };

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/TimeoutedSet");
} // unnamed namespace

namespace tut
{
// check adding simple element
template<>
template<>
void testObj::test<1>(void)
{
  HashSharedPtr hash(new Hash("key"));
  ts_.add(hash);
  ensure("Element not present in collection", ts_.isTimeouted(hash));
}

// check adding by Entry destructor
template<>
template<>
void testObj::test<2>(void)
{
  HashSharedPtr  hash(new Hash("key"));
  {
    EntrySharedPtr entry(new Entry(hash, bf_, ts_));
  }
  ensure("Element not present in collection", ts_.isTimeouted(hash));
}

// check pruning element not saved in Dynamic Config
template<>
template<>
void testObj::test<3>(void)
{
  HashSharedPtr hashPtr(new Hash("hash"));
  ts_.add(hashPtr);
  ensure("Element not present in collection", ts_.isTimeouted(hashPtr));
  ts_.markRemoved(bf_, owner_);
  ensure("Element present in collection after prune", ts_.isTimeouted(hashPtr) == false);
}

//TODO: check pruning element saved in Dynamic Config
template<>
template<>
void testObj::test<4>(void)
{
}

//TODO
template<>
template<>
void testObj::test<5>(void)
{
}

} // namespace tut
