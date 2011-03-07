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

    void testData(std::string key, std::string value)
    {
      TestDynamicConfigStub::Data::DataMap data = tconn_->data_->owner_["Filter::NewEvent"];
      tut::ensure_equals("invalid value", data[key], value );
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
  Entry::Hash hash("hash");
  ts_.add(hash);
  ensure("Element not present in collection", ts_.isTimeouted(hash));
}

// check adding by Entry destructor
template<>
template<>
void testObj::test<2>(void)
{
  Entry::Hash hash;
  {
    Entry e("key", &bf_, &ts_);
    hash = e.getHash();
  }
  ensure("Element not present in collection", ts_.isTimeouted(hash));
}

// check pruning element not saved in Dynamic Config
template<>
template<>
void testObj::test<3>(void)
{
  Entry::Hash hash("hash");
  ts_.add(hash);
  ensure("Element not present in collection", ts_.isTimeouted(hash));
  ts_.prune(&bf_, owner_);
  ensure("Element present in collection after prune", ts_.isTimeouted(hash) == false);
}

// check pruning element saved in Dynamic Config
template<>
template<>
void testObj::test<4>(void)
{
  Entry::Hash hash;
  {
    Entry e("key", &bf_, &ts_);
    hash = e.getHash();
  }
  testData(hash, "true");
  ensure("Element not present in collection", ts_.isTimeouted(hash));
  ts_.prune(&bf_, owner_);
  testData(hash, "");
  ensure("Element present in collection after prune", ts_.isTimeouted(hash) == false);
}
} // namespace tut
