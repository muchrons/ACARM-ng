/*
 * GatherIPs.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Algo/GatherIPs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"

using namespace std;
using namespace Algo;
using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public TestStubs
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Algo/GatherIPs");
} // unnamed namespace


namespace tut
{

// test getting something for empty alert
template<>
template<>
void testObj::test<1>(void)
{
  GatherIPs gip( makeNewLeaf( makeNewAlertWithHosts(NULL, NULL, NULL, NULL) ) );
  ensure_equals("source IPs found", gip.getSourceIPs().size(), 0u);
  ensure_equals("target IPs found", gip.getTargetIPs().size(), 0u);
}

// check if source IPs are recognized
template<>
template<>
void testObj::test<2>(void)
{
  GatherIPs gip( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL, NULL, NULL) ) );
  ensure_equals("source IPs not found", gip.getSourceIPs().size(), 1u);
  ensure_equals("target IPs found",     gip.getTargetIPs().size(), 0u);
}

// tets if destination IPs are recognized
template<>
template<>
void testObj::test<3>(void)
{
  GatherIPs gip( makeNewLeaf( makeNewAlertWithHosts(NULL, NULL, "1.2.3.4", NULL) ) );
  ensure_equals("source IPs found",     gip.getSourceIPs().size(), 0u);
  ensure_equals("target IPs not found", gip.getTargetIPs().size(), 1u);
}

// tets if non-unique entries are saved
template<>
template<>
void testObj::test<4>(void)
{
  GatherIPs gip( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", "2.3.4.5",
                                                    "2.3.4.5", "1.2.3.4") ) );
  ensure_equals("source IPs not found", gip.getSourceIPs().size(), 2u);
  ensure_equals("target IPs not found", gip.getTargetIPs().size(), 2u);
}

// test if unique IPs are counted
template<>
template<>
void testObj::test<5>(void)
{
  GatherIPs gip( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", "1.2.3.4",
                                                    NULL,      NULL) ) );
  ensure_equals("source IPs not found", gip.getSourceIPs().size(), 1u);
  ensure_equals("target IPs found",     gip.getTargetIPs().size(), 0u);
  ensure_equals("invalid IP saved", gip.getSourceIPs().begin()->first,
                                    Persistency::Host::IPv4::from_string("1.2.3.4") );
  ensure_equals("invalid IP count", gip.getSourceIPs().begin()->second.get(), 2u );
}

// smoke test passing throught bigger tree
template<>
template<>
void testObj::test<6>(void)
{
  GatherIPs gip( makeNewNode(
                   makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", "5.6.7.8",
                                                      "2.3.4.5", "6.7.8.9") ),
                   makeNewLeaf( makeNewAlertWithHosts("2.3.4.5", "1.2.3.4",
                                                      "2.3.4.5", "4.3.2.1") ) )
               );
  ensure_equals("source IPs not found", gip.getSourceIPs().size(), 3u);
  ensure_equals("target IPs not found", gip.getTargetIPs().size(), 3u);
}

// test counter's c-tor
template<>
template<>
void testObj::test<7>(void)
{
  ensure_equals("invalid initial counter's value", GatherIPs::Counter().get(), 0u);
}

// test pre-increment
template<>
template<>
void testObj::test<8>(void)
{
  GatherIPs::Counter c;
  const size_t tmp=++c;
  ensure_equals("invalid increment", c.get(), 1u);
  ensure_equals("invalid value returned", tmp, c.get() );
}

} // namespace tut
