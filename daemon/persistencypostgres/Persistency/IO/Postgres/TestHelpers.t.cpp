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

AlertPtr makeNewAlert(const char *name)
{
  const Persistency::Alert::SourceAnalyzers sa( makeNewAnalyzer() );
  return AlertPtr( new Persistency::Alert(name,
                             sa,
                             NULL,
                             Timestamp(),
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
                                        Persistency::Timestamp() ) );
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

GraphNodePtrNN makeNewLeaf(void)
{
  Persistency::IO::ConnectionPtrNN conn=Persistency::IO::create();
  IO::Transaction t( conn->createNewTransaction("make_leaf_transaction") );
  return GraphNodePtrNN( new Persistency::GraphNode( makeNewAlert(), conn, t) );
}

GraphNodePtrNN makeNewNode(void)
{
  return makeNewNode( makeNewLeaf(), makeNewLeaf() );
}

GraphNodePtrNN makeNewNode(GraphNodePtrNN child1, GraphNodePtrNN child2)
{
  Persistency::IO::ConnectionPtrNN conn=Persistency::IO::create();
  IO::Transaction t( conn->createNewTransaction("make_node_transaction") );
  const Persistency::NodeChildrenVector ncv(child1, child2);
  return GraphNodePtrNN( new Persistency::GraphNode( makeNewMetaAlert(),
                                                     conn, t, ncv) );
}

GraphNodePtrNN makeNewTree1(void)
{
  return makeNewNode( makeNewNode(),
                      makeNewNode(
                         makeNewNode(), makeNewLeaf() ) );
}

GraphNodePtrNN makeNewTree2(void)
{
  GraphNodePtrNN node1=makeNewNode();
  return makeNewNode( node1,
                      makeNewNode(
                        makeNewNode( makeNewLeaf(), node1 ), node1 ) );
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
