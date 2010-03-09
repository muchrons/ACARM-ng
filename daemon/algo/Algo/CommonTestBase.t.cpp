/*
 * CommonTestBase.t.cpp
 *
 */
#include "Algo/CommonTestBase.t.hpp"
#include <cassert>

using namespace Persistency;
using namespace TestHelpers::Persistency;

namespace Algo
{

HostPtrNN CommonTestBase::makeHost(const char *host) const
{
  assert(host!=NULL);
  const Host::Netmask_v4  mask(mask4_bytes);
  const char             *os="Linux";
  return makeNewHost4(host, &mask, os);
}

AlertPtrNN CommonTestBase::makeAlert(const char *hostSrc1,
                                     const char *hostSrc2,
                                     const char *hostDst1,
                                     const char *hostDst2) const
{
  const Alert::SourceAnalyzers  sa( makeNewAnalyzer() );
  Alert::ReportedHosts          hostsSrc;
  Alert::ReportedHosts          hostsDst;

  if(hostSrc1!=NULL)
    hostsSrc.push_back( makeHost(hostSrc1) );
  if(hostSrc2!=NULL)
    hostsSrc.push_back( makeHost(hostSrc2) );

  if(hostDst1!=NULL)
    hostsDst.push_back( makeHost(hostDst1) );
  if(hostDst2!=NULL)
    hostsDst.push_back( makeHost(hostDst2) );

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

GraphNodePtrNN CommonTestBase::makeLeaf(AlertPtrNN alert) const
{
  IO::ConnectionPtrNN conn( IO::create() );
  IO::Transaction     t( conn->createNewTransaction("make_leaf_trans") );
  return GraphNodePtrNN( new GraphNode(alert, conn, t) );
}

} // namespace Algo
