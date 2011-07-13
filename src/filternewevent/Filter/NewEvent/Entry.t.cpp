/*
 * Entry.t.cpp
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
  TimeoutedSet                           ts_;
  Persistency::IO::DynamicConfig::Owner  owner_;
  Persistency::IO::DynamicConfigAutoPtr  dc_;
  Hash                                   hash_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/NewEvent/Entry");
} // unnamed namespace

namespace tut
{

// test valid data saving to the Dynamic Config
template<>
template<>
void testObj::test<1>(void)
{
  EntrySharedPtr entryPtr(new Entry(hash_, bf_, ts_));
  ensure_equals("invalid value", string( dc_->read(entryPtr->getHashString().get())->get() ), "true");
}

// test if element is not present in Dynamic Config after TimeoutedSet prune
template<>
template<>
void testObj::test<2>(void)
{
  std::string hashStr;
  {
    EntrySharedPtr entryPtr(new Entry(hash_, bf_, ts_));
    hashStr=string(entryPtr.get()->getHashString().get());
  }
  // test if hash is in the TimeoutedSet
  ensure_equals("invalid value", string( dc_->read(hashStr)->get() ), "true");
  // clear timeouted set
  ts_.markRemoved(bf_, owner_);
  ensure("invalid value", dc_->read(hashStr).get() == NULL);
}

// test getHashString() method
template<>
template<>
void testObj::test<3>(void)
{
  const string sha1("a62f2225bf70bfaccbc7f1ef2a397836717377de");
  ensure_equals("invalid hash", hash_.getHash().get(), sha1);
}

// chaeck if hash name is present in the Timeouted Set after destroying Entry object
template<>
template<>
void testObj::test<4>(void)
{
  std::string hashStr;
  {
    EntrySharedPtr  entryPtr(new Entry(hash_, bf_, ts_));
    hashStr=string(entryPtr->getHashString().get());
  }
  ensure_equals("invalid value", string( dc_->read(hashStr)->get() ), "true");
  ensure("Element not present in collection", ts_.isTimeouted(hash_));
}

// test getHash() method
template<>
template<>
void testObj::test<5>(void)
{
  EntrySharedPtr  entryPtr(new Entry(hash_, bf_, ts_));
  ensure("invalid hash", entryPtr->getHash() == hash_);
}

} // namespace tut
