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
using namespace Filter;
using namespace Filter::NewEvent;

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

  // TODO: parameters should be const-refs.
  void testData(std::string key, std::string value)
  {
    TestDynamicConfigStub::Data::DataMap data = tconn_->data_->owner_["Filter::NewEvent"];  // TODO: separate this string to a constant
    tut::ensure_equals("invalid value", data[key], value );
  }

  TestConnection                        *tconn_;
  Persistency::IO::ConnectionPtrNN       conn_;
  BackendFacade::ChangedNodes            changed_;
  BackendFacade                          bf_;
  TimeoutedSet                           ts_;
  Persistency::IO::DynamicConfig::Owner  owner_;
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
  Entry e("key", &bf_, &ts_);
  testData(e.getHash(), string("true") );
}

// test if element is not present in Dynamic Config after TimeoutedSet prune
template<>
template<>
void testObj::test<2>(void)
{
  Entry e("key", &bf_, &ts_);
  testData(e.getHash(), string("true") );
  ts_.prune(&bf_, owner_);
  testData(e.getHash(), string("") );
}

// test getName() method
template<>
template<>
void testObj::test<3>(void)
{
  Entry e("key", &bf_, &ts_);
  ensure_equals("invalid name", e.getName(), "key");
}

// TODO: test getHash() method
template<>
template<>
void testObj::test<4>(void)
{
}

// chaeck if hash name is present in the Timeouted Set after destroying Entry object
template<>
template<>
void testObj::test<5>(void)
{
  Entry::Hash hash;
  {
    Entry e("key", &bf_, &ts_);
    hash = e.getHash();
  }
  testData(hash, "true");
  ensure("Element not present in collection", ts_.isTimeouted(hash));
}
} // namespace tut
