/*
 * GatherHosts.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Algo/GatherHosts.hpp"
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

factory tf("Algo/GatherHosts");
} // unnamed namespace


namespace tut
{

// test getting something for empty host
template<>
template<>
void testObj::test<1>(void)
{
  GatherHosts gh( makeNewLeaf( makeNewAlertWithHosts(NULL, NULL, NULL, NULL) ) );
  ensure_equals("source hosts found", gh.getSourceHosts().size(), 0);
  ensure_equals("target hosts found", gh.getTargetHosts().size(), 0);
}

// check if source hosts are recognized
template<>
template<>
void testObj::test<2>(void)
{
  GatherHosts gh( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL, NULL, NULL) ) );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 1);
  ensure_equals("target hosts found",     gh.getTargetHosts().size(), 0);
}

// tets if destination hosts are recognized
template<>
template<>
void testObj::test<3>(void)
{
  GatherHosts gh( makeNewLeaf( makeNewAlertWithHosts(NULL, NULL, "1.2.3.4", NULL) ) );
  ensure_equals("source hosts found",     gh.getSourceHosts().size(), 0);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 1);
}

// tets if non-unique entries are saved
template<>
template<>
void testObj::test<4>(void)
{
  GatherHosts gh( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", "2.3.4.5",
                                                     "2.3.4.5", "1.2.3.4") ) );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 2);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 2);
}

// test if unique hosts are gathered as one
template<>
template<>
void testObj::test<5>(void)
{
  GatherHosts gh( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", "1.2.3.4",
                                                     NULL,      NULL) ) );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 1);
  ensure_equals("target hosts found",     gh.getTargetHosts().size(), 0);
}

// smoke test passing throught bigger tree
template<>
template<>
void testObj::test<6>(void)
{
  GatherHosts gh( makeNewNode(
                    makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", "5.6.7.8",
                                                       "2.3.4.5", "6.7.8.9") ),
                    makeNewLeaf( makeNewAlertWithHosts("2.3.4.5", "1.2.3.4",
                                                       "2.3.4.5", "4.3.2.1") ) )
                );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 3);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 3);
}

// test returned values
template<>
template<>
void testObj::test<7>(void)
{
  const HostPtrNN h1=makeNewHost("1.2.3.4");
  const HostPtrNN h2=makeNewHost("9.8.7.6");
  GatherHosts gh( makeNewLeaf( makeNewAlertWithHosts("1.2.3.4", NULL,
                                                     "9.8.7.6", NULL) ) );
  // check numbers
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 1);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 1);
  // check values
  ensure("invalid source IP", (*gh.getSourceHosts().begin())->getIP()==h1->getIP() );
  ensure("invalid target IP", (*gh.getTargetHosts().begin())->getIP()==h2->getIP() );
}

// test SWO object for inequality
template<>
template<>
void testObj::test<8>(void)
{
  const HostPtrNN            h1=makeNewHost("1.2.3.4");
  const HostPtrNN            h2=makeNewHost("9.8.7.6");
  const GatherHosts::HostSWO swo=GatherHosts::HostSWO();
  ensure("invalid compare result for < operation",   swo(h1, h2) );
  ensure("invalid compare result for !< operation", !swo(h2, h1) );
}

// test SWO object for equivalence
template<>
template<>
void testObj::test<9>(void)
{
  const HostPtrNN            h1=makeNewHost("1.2.3.4");
  const HostPtrNN            h2=makeNewHost("1.2.3.4");
  const GatherHosts::HostSWO swo=GatherHosts::HostSWO();
  ensure("invalid equality compare 1", !swo(h1, h2) );
  ensure("invalid equality compare 2", !swo(h2, h1) );
}

} // namespace tut
