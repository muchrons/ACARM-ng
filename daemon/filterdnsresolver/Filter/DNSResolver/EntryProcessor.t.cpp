/*
 * EntryProcessor.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Persistency/IO/BackendFactory.hpp"
#include "Filter/DNSResolver/EntryProcessor.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter;
using namespace Filter::DNSResolver;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestStubs
{
  TestClass(void):
    conn_( Persistency::IO::create() ),
    cache_(42),
    bp_(conn_, changed_, "testdnsresolver"),
    ep_(&cache_, &bp_)
  {
    assert( changed_.size()==0 );
  }

  Persistency::GraphNodePtrNN makeLeaf(const char *sip1,
                                       const char *sip2,
                                       const char *tip1,
                                       const char *tip2,
                                       const char *dns=NULL)
  {
    const ::Persistency::Alert::SourceAnalyzers sa( makeNewAnalyzer() );
    AlertPtrNN alert( new Alert("some alert name",
                                sa,
                                NULL,
                                Timestamp(),
                                Severity(SeverityLevel::INFO),
                                Certainty(0.42),
                                "some test allert",
                                makeReportedHosts(sip1, sip2, dns),
                                makeReportedHosts(tip1, tip2, dns) ) );
    IO::Transaction t( conn_->createNewTransaction("make_leaf_transaction") );
    return GraphNodePtrNN( new GraphNode(alert, conn_, t) );
  }

  Alert::ReportedHosts makeReportedHosts(const char *ip1,
                                         const char *ip2,
                                         const char *dns=NULL) const
  {
    Alert::ReportedHosts rh;
    if(ip1!=NULL)
      rh.push_back( makeHostIP(ip1, dns) );
    if(ip2!=NULL)
      rh.push_back( makeHostIP(ip2, dns) );
    return rh;
  }

  HostPtrNN makeHostIP(const char *ip, const char *dns=NULL) const
  {
    assert(ip!=NULL);
    return HostPtrNN( new Host( Host::IPv4::from_string(ip),
                                NULL,
                                "penguin",
                                makeNewReferenceURL(),
                                Host::ReportedServices(),
                                Host::ReportedProcesses(),
                                dns ) );
  }

  Persistency::IO::ConnectionPtrNN conn_;
  CachedDNS                        cache_;
  BackendProxy::ChangedNodes       changed_;
  BackendProxy                     bp_;
  EntryProcessor                   ep_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/EntryProcessor");
} // unnamed namespace


namespace tut
{

// test empty run (no hosts)
template<>
template<>
void testObj::test<1>(void)
{
  ep_( makeNewLeaf() );
  ensure_equals("something has been changed", changed_.size(), 0);
}

// test run when some source hosts have reverse dns'
template<>
template<>
void testObj::test<2>(void)
{
  ep_( makeLeaf("127.0.0.1", NULL, NULL, NULL) );
  ensure_equals("nothing has been changed", changed_.size(), 1);
  ensure_equals("invalid dns found",
                changed_.at(0)->getAlert()->getReportedSourceHosts().at(0)->getName().get(),
                string("localhost") );
}

// test run when there are hosts but no reverse dns mappings
template<>
template<>
void testObj::test<3>(void)
{
  ep_( makeLeaf("192.168.255.254", NULL, "192.168.255.253", NULL) );
  ensure_equals("something has been changed", changed_.size(), 0);
}

// test run when some target hosts have reverse dns'
template<>
template<>
void testObj::test<4>(void)
{
  ep_( makeLeaf(NULL, NULL, "127.0.0.1", NULL) );
  ensure_equals("nothing has been changed", changed_.size(), 1);
  ensure_equals("invalid dns found",
                changed_.at(0)->getAlert()->getReportedTargetHosts().at(0)->getName().get(),
                string("localhost") );
}

} // namespace tut
