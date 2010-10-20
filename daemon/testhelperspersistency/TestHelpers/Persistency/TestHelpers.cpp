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

AlertPtr makeNewAlert(const char   *name,
                      const char   *sip,
                      const char   *tip,
                      const char   *dns,
                      const double  certainty)
{
  const ::Persistency::Alert::Analyzers sa( makeNewAnalyzer() );
  Alert::Hosts srcHosts;
  Alert::Hosts tgtHosts;

  if(sip!=NULL)
    srcHosts.push_back( makeNewHost(sip, dns) );
  if(tip!=NULL)
    tgtHosts.push_back( makeNewHost(tip, dns) );

  return AlertPtr( new Alert(name,
                             sa,
                             NULL,
                             Timestamp(12345),
                             Severity(SeverityLevel::INFO),
                             Certainty(certainty),
                             "some test alert",
                             srcHosts,
                             tgtHosts) );
}

MetaAlertPtr makeNewMetaAlert(const char *name, const unsigned int id)
{
  return MetaAlertPtrNN( new MetaAlert( MetaAlert::Name(name),
                                        0.1, 0.2,
                                        makeNewReferenceURL(),
                                        Timestamp(),
                                        id ) );
}

AnalyzerPtrNN makeNewAnalyzer(const char *name)
{
  return AnalyzerPtrNN( new Analyzer(42u, name, NULL, NULL, NULL) );
}

HostPtr makeNewHost(const char *ip, const char *dns)
{
  const Host::Netmask_v4 mask(mask4_bytes);
  return makeNewHost4(ip, &mask, "linux", false, dns);
}

HostPtr makeNewHost4(const char             *ip,
                     const Host::Netmask_v4 *mask,
                     const char             *os,
                     bool                    nullRef,
                     const char             *dns)
{
  return HostPtr( new Host( Host::IPv4::from_string(ip),
                            mask,
                            os,
                            (nullRef)?(ReferenceURLPtr()):(makeNewReferenceURL()),
                            Host::Services(),
                            Host::Processes(),
                            dns ) );
}

HostPtr makeNewHost6(const char             *ip,
                     const Host::Netmask_v6 *mask,
                     const char             *os,
                     bool                    nullRef,
                     const char             *dns)
{
  return HostPtr( new Host( Host::IPv6::from_string(ip),
                            mask,
                            os,
                            (nullRef)?(ReferenceURLPtr()):(makeNewReferenceURL()),
                            Host::Services(),
                            Host::Processes(),
                            dns ) );
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

GraphNodePtrNN makeNewLeaf(const char *sip, const char *tip, const bool dns, const double certainty)
{
  const char *name=dns?"dns.org":NULL;
  return makeNewLeaf( makeNewAlert("some alert", sip, tip, name, certainty) );
}

GraphNodePtrNN makeNewNode(void)
{
  return makeNewNode( makeNewLeaf(), makeNewLeaf() );
}

GraphNodePtrNN makeNewNode(GraphNodePtrNN child1, GraphNodePtrNN child2, unsigned int id)
{
  ::Persistency::IO::ConnectionPtrNN conn( ::Persistency::IO::create() );
  IO::Transaction t( conn->createNewTransaction("make_node_transaction") );
  const ::Persistency::NodeChildrenVector ncv(child1, child2);
  GraphNodePtrNN graphNode( new GraphNode( makeNewMetaAlert("some meta-alert", id),
                                           conn, t, ncv) );
  t.commit();
  return graphNode;
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

AlertPtrNN makeNewAlertWithHosts(const char *hostSrc1,
                                 const char *hostSrc2,
                                 const char *hostDst1,
                                 const char *hostDst2)
{
  const Alert::Analyzers sa( makeNewAnalyzer() );
  Alert::Hosts           hostsSrc;
  Alert::Hosts           hostsDst;

  if(hostSrc1!=NULL)
    hostsSrc.push_back( makeNewHost(hostSrc1) );
  if(hostSrc2!=NULL)
    hostsSrc.push_back( makeNewHost(hostSrc2) );

  if(hostDst1!=NULL)
    hostsDst.push_back( makeNewHost(hostDst1) );
  if(hostDst2!=NULL)
    hostsDst.push_back( makeNewHost(hostDst2) );

  return AlertPtrNN( new Alert("alert 123",
                               sa,
                               NULL,
                               Timestamp(),
                               Severity(SeverityLevel::INFO),
                               Certainty(0.42),
                               "some test allert",
                               hostsSrc,
                               hostsDst) );
}

GraphNodePtrNN makeNewLeaf(AlertPtrNN alert, const unsigned int id)
{
  IO::ConnectionPtrNN conn( IO::create() );
  IO::Transaction     t( conn->createNewTransaction("make_leaf_trans") );
  GraphNodePtrNN graphNode( new GraphNode(alert, id, conn, t) );
  t.commit();
  return graphNode;
}

} // namespace Persistency
} // namespace TestHelpers
