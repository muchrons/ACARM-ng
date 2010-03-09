/*
 * Strategy.t.cpp
 *
 */
#include <tut.h>
#include <string>
#include <cassert>

#include "Filter/DNSResolver/Strategy.hpp"
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestStubs.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace std;
using namespace Persistency;
using namespace Filter::DNSResolver;
using namespace TestHelpers::Persistency;

namespace
{

struct TestClass: private TestHelpers::Persistency::TestStubs
{
  TestClass(void):
    s_( Strategy::Parameters(42) )
  {
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

  AlertPtrNN makeAlertWithHost(const char *ip) const
  {
    const Alert::SourceAnalyzers sa( makeNewAnalyzer() );
    Alert::ReportedHosts         hosts;
    hosts.push_back( makeHostIP(ip) );
    return AlertPtrNN( new Alert("alert 123",
                                 sa,
                                 NULL,
                                 Timestamp(),
                                 Severity(SeverityLevel::INFO),
                                 Certainty(0.42),
                                 "some test allert",
                                 hosts,
                                 Alert::ReportedHosts() ) );
  }

  Persistency::GraphNodePtrNN makeLeaf(const char *ip) const
  {
    assert(ip!=NULL);
    IO::ConnectionPtrNN conn( Persistency::IO::create() );
    IO::Transaction     t( conn->createNewTransaction("make_leaf_trans") );
    return GraphNodePtrNN( new GraphNode( makeAlertWithHost(ip), conn, t) );
  }

  Persistency::GraphNodePtrNN makeNode(void) const
  {
    return makeNewNode( makeLeaf("127.0.0.1"), makeLeaf("127.0.0.1") );
  }

  Strategy::ChangedNodes changed_;
  Strategy               s_;
};

typedef tut::test_group<TestClass> factory;
typedef factory::object testObj;

factory tf("Filter/DNSResolver/Strategy");
} // unnamed namespace


namespace tut
{

// test leaf
template<>
template<>
void testObj::test<1>(void)
{
  s_.process( makeLeaf("127.0.0.1"), changed_ );
  ensure_equals("nothing changed", changed_.size(), 1);
}

// test node
template<>
template<>
void testObj::test<2>(void)
{
  s_.process( makeNode(), changed_ );
  ensure_equals("nothing changed", changed_.size(), 2);
}

} // namespace tut
