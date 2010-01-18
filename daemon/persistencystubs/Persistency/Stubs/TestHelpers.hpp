/*
 * TestHelpers.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_TESTHELPERS_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_TESTHELPERS_HPP_FILE

/* public header */

#include "Persistency/Alert.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Certanity.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/MD5Sum.hpp"
#include "Persistency/MetaAlert.hpp"
#include "Persistency/Process.hpp"
#include "Persistency/ReferenceURL.hpp"
#include "Persistency/Service.hpp"
#include "Persistency/Severity.hpp"
#include "Persistency/SeverityLevel.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/GraphNode.hpp"


namespace Persistency
{
namespace Stubs
{

AlertPtr makeNewAlert(const char *name="some alert");

MetaAlertPtr makeNewMetaAlert(const char *name="some meta-alert");

AnalyzerPtr makeNewAnalyzer(const char *name="some analyzer");
AnalyzerPtr makeNewAnalyzer(const char *name, HostPtr host);

const Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                  0xFF, 0xFF, 0xFF, 0xFF,
                                                  0xFF, 0xFF, 0xFF, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                 } };
HostPtr makeNewHost(void);
HostPtr makeNewHost4(const char             *ip,
                     const Host::Netmask_v4 *mask,
                     const char             *os,
                     bool                    nullRef=false);
HostPtr makeNewHost6(const char             *ip,
                     const Host::Netmask_v6 *mask,
                     const char             *os,
                     bool                    nullRef=false);

ProcessPtr makeNewProcess(const char *name="process name");

ReferenceURLPtr makeNewReferenceURL(const char *url="http://gnu.org");

GraphNodePtrNN makeNewLeaf(void);
GraphNodePtrNN makeNewNode(void);
GraphNodePtrNN makeNewNode(GraphNodePtrNN child1, GraphNodePtrNN child2);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      leaf5
//                leaf3 leaf4
//
GraphNodePtrNN makeNewTree1(void);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      node1
//                leaf3 node1    ...
//                       ...
//
GraphNodePtrNN makeNewTree2(void);

} // namespace Stubs
} // namespace Persistency

#endif
