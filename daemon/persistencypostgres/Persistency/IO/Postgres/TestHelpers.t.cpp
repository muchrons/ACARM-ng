/*
 * TestHelpers.t.cpp
 *
 */
#include "Persistency/IO/BackendFactory.hpp"
#include "Persistency/IO/Postgres/TestHelpers.t.hpp"


namespace Persistency
{
namespace IO
{
namespace Postgres
{

IO::ConnectionPtrNN makeConnection(void)
{
  IO::BackendFactory::Options opts;
  opts["host"]  ="localhost";
  opts["port"]  ="5432";
  opts["dbname"]="acarm_ng_test";
  opts["user"]  ="acarm-ng-daemon";
  opts["pass"]  ="test.daemon";
  return IO::ConnectionPtrNN(
        Persistency::IO::BackendFactory::create("postgres", opts) );
}

AlertPtr makeNewAlert(const char *name)
{
  const Persistency::Alert::SourceAnalyzers sa( makeNewAnalyzer() );
  return AlertPtr( new Persistency::Alert(name,
                             sa,
                             NULL,
                             Timestamp(444),
                             Severity(SeverityLevel::INFO),
                             Certainty(0.42),
                             "some test allert",
                             Persistency::Alert::ReportedHosts(),
                             Persistency::Alert::ReportedHosts() ) );
}

MetaAlertPtr makeNewMetaAlert(const char *name)
{
  return MetaAlertPtrNN( new Persistency::MetaAlert( Persistency::MetaAlert::Name(name),
                                        0.1, 0.2,
                                        makeNewReferenceURL(),
                                        Persistency::Timestamp(444) ) );
}

AnalyzerPtrNN makeNewAnalyzer(const char *name)
{
  return AnalyzerPtrNN( new Persistency::Analyzer(name, NULL, NULL, NULL) );
}

HostPtr makeNewHost(void)
{
  const Persistency::Host::Netmask_v4 mask(mask4_bytes);
  return makeNewHost4("1.2.3.4", &mask, "linux");
}

HostPtr makeNewHost4(const char             *ip,
                     const Persistency::Host::Netmask_v4 *mask,
                     const char             *os,
                     bool                    nullRef)
{
  return HostPtr( new Persistency::Host( Persistency::Host::IPv4::from_string(ip),
                            mask,
                            os,
                            (nullRef)?(ReferenceURLPtr()):(makeNewReferenceURL()),
                            Persistency::Host::ReportedServices(),
                            Persistency::Host::ReportedProcesses(),
                            "dns.org" ) );
}

HostPtr makeNewHost6(const char             *ip,
                     const Persistency::Host::Netmask_v6 *mask,
                     const char             *os,
                     bool                    nullRef)
{
  return HostPtr( new Persistency::Host( Persistency::Host::IPv6::from_string(ip),
                            mask,
                            os,
                            (nullRef)?(ReferenceURLPtr()):(makeNewReferenceURL()),
                            Persistency::Host::ReportedServices(),
                            Persistency::Host::ReportedProcesses(),
                            "dns.org" ) );
}

ProcessPtr makeNewProcess(const char *name)
{
  return ProcessPtr( new Persistency::Process( "/path/to/bin",
                                 name,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL,
                                 NULL,
                                 ReferenceURLPtr() ) );
}

ReferenceURLPtr makeNewReferenceURL(const char *url)
{
  return ReferenceURLPtr( new Persistency::ReferenceURL("some name", url) );
}

GraphNodePtrNN makeNewLeaf(const char *name)
{
  Persistency::IO::ConnectionPtrNN conn( Persistency::IO::create() );
  IO::Transaction t( conn->createNewTransaction("make_leaf_transaction") );
  GraphNodePtrNN graphNode( new Persistency::GraphNode( makeNewAlert(name), conn, t) );
  t.commit();
  return graphNode;
}

GraphNodePtrNN makeNewNode(void)
{
  return makeNewNode( makeNewLeaf("some name 1"), makeNewLeaf("some name 2"), "another meta alert name");
}

GraphNodePtrNN makeNewNode(GraphNodePtrNN child1, GraphNodePtrNN child2)
{
  Persistency::IO::ConnectionPtrNN conn( Persistency::IO::create() );
  IO::Transaction t( conn->createNewTransaction("make_node_transaction") );
  const Persistency::NodeChildrenVector ncv(child1, child2);
  GraphNodePtrNN graphNode( new Persistency::GraphNode( makeNewMetaAlert("bla"),
                                                     conn, t, ncv) );
  t.commit();
  return graphNode;
}

GraphNodePtrNN makeNewNode(GraphNodePtrNN child1, GraphNodePtrNN child2, const char *name)
{
  Persistency::IO::ConnectionPtrNN conn( Persistency::IO::create() );
  IO::Transaction t( conn->createNewTransaction("make_node_transaction") );
  const Persistency::NodeChildrenVector ncv(child1, child2);
  GraphNodePtrNN graphNode( new Persistency::GraphNode( makeNewMetaAlert(name),
                                                     conn, t, ncv) );
  t.commit();
  return graphNode;
}

GraphNodePtrNN makeNewTree1(void)
{
  return makeNewNode( makeNewNode(makeNewLeaf("leaf1"), makeNewLeaf("leaf2"), "node1" ),
                      makeNewNode(
                         makeNewNode(makeNewLeaf("leaf3"),
                                     makeNewLeaf("leaf4"),
                                     "node3"), makeNewLeaf("leaf5"), "node2"), "root" );
}

GraphNodePtrNN makeNewTree2(void)
{
  GraphNodePtrNN node1=makeNewNode();
  return makeNewNode( node1,
                      makeNewNode(
                        makeNewNode( makeNewLeaf(), node1 ), node1 ) );
}

/*
GraphNodePtrNN makeNewTree3(void)
{
  GraphNodePtrNN node6 = makeNewNode();
  GraphNodePtrNN node7 = makeNewNode();
  GraphNodePtrNN node8 = makeNewLeaf();
  GraphNodePtrNN node5 = makeNewLeaf();

  GraphNodePtrNN node6 = makeNewNode();

}
*/
} // namespace Postgres
} // namespace IO
} // namespace Persistency
