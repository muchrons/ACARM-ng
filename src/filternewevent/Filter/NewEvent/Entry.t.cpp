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

factory tf("Filter/NewEvent/Entry");
} // unnamed namespace

namespace tut
{

// TODO
template<>
template<>
void testObj::test<1>(void)
{
  Entry e("key", &bf_, &ts_);
  testData(e.getHash(), string("true") );
}

// TODO
template<>
template<>
void testObj::test<2>(void)
{
  Entry e("key", &bf_, &ts_);
  testData(e.getHash(), string("true") );
  ts_.prune(&bf_, owner_);
  testData(e.getHash(), string("") );
}

} // namespace tut
