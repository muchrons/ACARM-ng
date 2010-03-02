/*
 * GatherHosts.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/ManyToMany/GatherHosts.hpp"
#include "Filter/ManyToMany/CommonTestBase.t.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::ManyToMany;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: public CommonTestBase
{
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/ManyToMany/GatherHosts");
} // unnamed namespace


namespace tut
{

// test getting something for empty host
template<>
template<>
void testObj::test<1>(void)
{
  GatherHosts gh( makeLeaf( makeAlert(NULL, NULL, NULL, NULL) ) );
  ensure_equals("source hosts found", gh.getSourceHosts().size(), 0);
  ensure_equals("target hosts found", gh.getTargetHosts().size(), 0);
}

// check if source hosts are recognized
template<>
template<>
void testObj::test<2>(void)
{
  GatherHosts gh( makeLeaf( makeAlert("1.2.3.4", NULL, NULL, NULL) ) );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 1);
  ensure_equals("target hosts found",     gh.getTargetHosts().size(), 0);
}

// tets if destination hosts are recognized
template<>
template<>
void testObj::test<3>(void)
{
  GatherHosts gh( makeLeaf( makeAlert(NULL, NULL, "1.2.3.4", NULL) ) );
  ensure_equals("source hosts found",     gh.getSourceHosts().size(), 0);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 1);
}

// tets if non-unique entries are saved
template<>
template<>
void testObj::test<4>(void)
{
  GatherHosts gh( makeLeaf( makeAlert("1.2.3.4", "2.3.4.5",
                                      "2.3.4.5", "1.2.3.4") ) );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 2);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 2);
}

// test if unique hosts are gathered as one
template<>
template<>
void testObj::test<5>(void)
{
  GatherHosts gh( makeLeaf( makeAlert("1.2.3.4", "1.2.3.4", NULL, NULL) ) );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 1);
  ensure_equals("target hosts found",     gh.getTargetHosts().size(), 0);
}

// smoke test passing throught bigger tree
template<>
template<>
void testObj::test<6>(void)
{
  GatherHosts gh( makeNewNode(
                    makeLeaf( makeAlert("1.2.3.4", "5.6.7.8", "2.3.4.5", "6.7.8.9") ),
                    makeLeaf( makeAlert("2.3.4.5", "1.2.3.4", "2.3.4.5", "4.3.2.1") ) )
                );
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 3);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 3);
}

// test returned values
template<>
template<>
void testObj::test<7>(void)
{
  const HostPtrNN h1=makeHost("1.2.3.4");
  const HostPtrNN h2=makeHost("9.8.7.6");
  GatherHosts gh( makeLeaf( makeAlert("1.2.3.4", NULL, "9.8.7.6", NULL) ) );
  // check numbers
  ensure_equals("source hosts not found", gh.getSourceHosts().size(), 1);
  ensure_equals("target hosts not found", gh.getTargetHosts().size(), 1);
  // check values
  ensure("invalid source IP", (*gh.getSourceHosts().begin())->getIP()==h1->getIP() );
  ensure("invalid target IP", (*gh.getTargetHosts().begin())->getIP()==h2->getIP() );
}

} // namespace tut
