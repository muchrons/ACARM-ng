/*
 * IDCache.t.cpp
 *
 */
#include <tut.h>

#include "Persistency/IO/Postgres/IDCache.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"

using namespace std;
using namespace Persistency::IO::Postgres;

namespace
{

struct TestClass
{
  TestClass(void):
    host_( makeNewHost() ),
    alert_( makeNewAlert() ),
    metaAlert_( makeNewMetaAlert() )
  {
  }

  void fill(void)
  {
    idc_.add(host_,      1);
    idc_.add(alert_,     3);
    idc_.add(metaAlert_, 5);
  }

  Persistency::HostPtrNN      host_;
  Persistency::AlertPtrNN     alert_;
  Persistency::MetaAlertPtrNN metaAlert_;
  IDCache                     idc_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Persistency/IO/Postgres/IDCache");
} // unnamed namespace


namespace tut
{

// test add/get host
template<>
template<>
void testObj::test<1>(void)
{
  idc_.add(host_, 42);
  ensure_equals("invalid host", idc_.get(host_), 42);
}

// test add/get alert
template<>
template<>
void testObj::test<2>(void)
{
  idc_.add(alert_, 42);
  ensure_equals("invalid alert", idc_.get(alert_), 42);
}

// test add/get meta-alert
template<>
template<>
void testObj::test<3>(void)
{
  idc_.add(metaAlert_, 42);
  ensure_equals("invalid meta-alert", idc_.get(metaAlert_), 42);
}

// test prunning nonempty elements
template<>
template<>
void testObj::test<4>(void)
{
  fill();
  // test
  idc_.prune();
  ensure_equals("invalid host",       idc_.get(host_),      1);
  ensure_equals("invalid alert",      idc_.get(alert_),     3);
  ensure_equals("invalid meta-alert", idc_.get(metaAlert_), 5);
}

// test prunning elements when they're NULLs
template<>
template<>
void testObj::test<5>(void)
{
  fill();
  host_     =makeNewHost();
  alert_    =makeNewAlert();
  metaAlert_=makeNewMetaAlert();
}

// test creating instance of IDCachePtrNN - smoke test.
template<>
template<>
void testObj::test<6>(void)
{
  IDCachePtrNN ptr(new IDCache);
}

} // namespace tut
