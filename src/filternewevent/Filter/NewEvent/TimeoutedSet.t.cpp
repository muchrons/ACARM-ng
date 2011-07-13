/*
 * TimeoutedSet.t.cpp
 *
 */
#include <tut.h>

#include "Filter/NewEvent/Entry.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "TestHelpers/Persistency/ConnectionIOMemory.hpp"

using namespace std;
using namespace Core::Types::Proc;
using namespace Filter;
using namespace Filter::NewEvent;
using namespace TestHelpers::Persistency;

namespace
{
struct TestClass
{
  TestClass(void):
    conn_( new ConnectionIOMemory ),
    bf_(conn_, changed_, TypeName("testnewevent"), InstanceName("myname")),
    owner_("Filter::NewEvent"),
    dc_(bf_.createDynamicConfig(owner_)),
    hash_("key")
  {
  }

  Persistency::IO::ConnectionPtrNN       conn_;
  BackendFacade::ChangedNodes            changed_;
  BackendFacade                          bf_;
  Persistency::IO::DynamicConfig::Owner  owner_;
  Persistency::IO::DynamicConfigAutoPtr  dc_;
  Hash                                   hash_;
  TimeoutedSet                           ts_;
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
  ts_.add(hash_);
  ensure("Element not present in collection", ts_.isTimeouted(hash_));
}

// check addition from Entry's destructor
template<>
template<>
void testObj::test<2>(void)
{
  {
    EntrySharedPtr entry(new Entry(hash_, bf_, ts_));
  }
  ensure("Element not present in collection", ts_.isTimeouted(hash_));
}

// check pruning element not saved in Dynamic Config
template<>
template<>
void testObj::test<3>(void)
{
  ts_.add(hash_);
  ensure("Element not present in collection", ts_.isTimeouted(hash_));
  ts_.markRemoved(bf_, owner_);
  ensure("Element present in collection after prune", ts_.isTimeouted(hash_) == false);
}

// check pruning element saved in Dynamic Config
template<>
template<>
void testObj::test<4>(void)
{
  std::string hashStr;
  {
    EntrySharedPtr  entryPtr(new Entry(hash_, bf_, ts_));
    hashStr = string(entryPtr->getHashString().get());
  }
  // test if hash is in the TimeoutedSet
  ensure_equals("invalid value", string( dc_->read(hashStr)->get() ), "true");
  // clear timeouted set
  ts_.markRemoved(bf_, owner_);
  ensure("invalid value", dc_->read(hashStr).get() == NULL);
}

// test if no exceptions are thrown from inside the 'makrRemoved()' call.
template<>
template<>
void testObj::test<5>(void)
{
  ts_.add(hash_);
  ensure("Element not present in collection", ts_.isTimeouted(hash_));
  try
  {
    ts_.markRemoved(bf_, owner_);
  }
  catch(...)
  {
    tut::fail("markRemowed() throw exception");
  }
  ensure("Element present in collection after prune", ts_.isTimeouted(hash_) == false);
}

} // namespace tut
