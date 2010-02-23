/*
 * TestHelpers.cpp
 *
 */
#include "Persistency/IO/BackendFactory.hpp"
#include "TestHelpers/Persistency/TestHelpers.hpp"

using namespace ::Persistency;

namespace TestHelpers
{
namespace Persistency
{

AlertPtr makeNewAlert(const char *name)
{
  const ::Persistency::Alert::SourceAnalyzers sa( makeNewAnalyzer() );
  return AlertPtr( new Alert(name,
                             sa,
                             NULL,
                             Timestamp(),
                             Severity(SeverityLevel::INFO),
                             Certainty(0.42),
                             "some test allert",
                             Alert::ReportedHosts(),
                             Alert::ReportedHosts() ) );
}

MetaAlertPtr makeNewMetaAlert(const char *name)
{
  return MetaAlertPtrNN( new MetaAlert( MetaAlert::Name(name),
                                        0.1, 0.2,
                                        makeNewReferenceURL(),
                                        Timestamp() ) );
}

AnalyzerPtrNN makeNewAnalyzer(const char *name)
{
  return AnalyzerPtrNN( new Analyzer(name, NULL, NULL, NULL) );
}

HostPtr makeNewHost(void)
{
  const Host::Netmask_v4 mask(mask4_bytes);
  return makeNewHost4("1.2.3.4", &mask, "linux");
}

HostPtr makeNewHost4(const char             *ip,
                     const Host::Netmask_v4 *mask,
                     const char             *os,
                     bool                    nullRef)
{
  return HostPtr( new Host( Host::IPv4::from_string(ip),
                            mask,
                            os,
                            (nullRef)?(ReferenceURLPtr()):(makeNewReferenceURL()),
                            Host::ReportedServices(),
                            Host::ReportedProcesses() ) );
}

HostPtr makeNewHost6(const char             *ip,
                     const Host::Netmask_v6 *mask,
                     const char             *os,
                     bool                    nullRef)
{
  return HostPtr( new Host( Host::IPv6::from_string(ip),
                            mask,
                            os,
                            (nullRef)?(ReferenceURLPtr()):(makeNewReferenceURL()),
                            Host::ReportedServices(),
                            Host::ReportedProcesses() ) );
}

ProcessPtr makeNewProcess(const char *name)
{
  return ProcessPtr( new Process( "/path/to/bin",
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
  return ReferenceURLPtr( new ReferenceURL("some name", url) );
}

GraphNodePtrNN makeNewLeaf(void)
{
  ::Persistency::IO::ConnectionPtrNN conn=::Persistency::IO::create();
  IO::Transaction t( conn->createNewTransaction("make_leaf_transaction") );
  return GraphNodePtrNN( new GraphNode( makeNewAlert(), conn, t) );
}

GraphNodePtrNN makeNewNode(void)
{
  return makeNewNode( makeNewLeaf(), makeNewLeaf() );
}

GraphNodePtrNN makeNewNode(GraphNodePtrNN child1, GraphNodePtrNN child2)
{
  ::Persistency::IO::ConnectionPtrNN conn=::Persistency::IO::create();
  IO::Transaction t( conn->createNewTransaction("make_node_transaction") );
  return GraphNodePtrNN( new GraphNode( makeNewMetaAlert(),
                                        conn, t,
                                        child1, child2 ) );
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

} // namespace Persistency
} // namespace TestHelpers
