/*
 * Entry.t.cpp
 *
 */

#include <tut.h>

#include "Filter/NewEvent/Entry.hpp"
#include "Filter/NewEvent/TimeoutedSet.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/DynamicConfig.hpp"
#include "Filter/NewEvent/TestConnection.t.hpp"

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
    tconn_(new TestConnection),
    conn_( tconn_ ),
    bf_(conn_, changed_, TypeName("testnewevent"), InstanceName("myname")),
    owner_("Filter::NewEvent"),
    hash_("key")
    //entryPtr_(new Entry(hash_, bf_, ts_))
    //hashStr_(entryPtr_.get()->getHash().get())
  {
  }

  void testData(const std::string &key, const std::string &value)
  {
    IODynamicConfigMemory::Memory data = tconn_->data_;  // TODO: separate this string to a constant
    tut::ensure_equals("invalid value", data[key], value );
  }

  TestConnection                        *tconn_;
  Persistency::IO::ConnectionPtrNN       conn_;
  BackendFacade::ChangedNodes            changed_;
  BackendFacade                          bf_;
  TimeoutedSet                           ts_;
  Persistency::IO::DynamicConfig::Owner  owner_;
  Hash                                   hash_;
  //EntrySharedPtr                         entryPtr_;
  //std::string                            hashStr_;

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
  testData( entryPtr.get()->getHash().get(), string("true") );
}

// test if element is not present in Dynamic Config after TimeoutedSet prune
template<>
template<>
void testObj::test<2>(void)
{
  std::string   hashStr;
  Hash          hash("some key");
  {
    EntrySharedPtr  entryPtr(new Entry(hash, bf_, ts_));
    hashStr = string(entryPtr.get()->getHash().get());
  }
  // test if hash is in the TimeoutedSet
  testData( hashStr, string("true") );
  // clear timeouted set
  ts_.markRemoved(bf_, owner_);
  testData( hashStr, string("") );
}

// test getHash() method
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
  std::string  hashStr;
  Hash         hash("some key");
  {
    EntrySharedPtr  entryPtr(new Entry(hash, bf_, ts_));
    hashStr = string(entryPtr.get()->getHash().get());
  }
  testData( hashStr, "true");
  ensure("Element not present in collection", ts_.isTimeouted(hash));
}

} // namespace tut
