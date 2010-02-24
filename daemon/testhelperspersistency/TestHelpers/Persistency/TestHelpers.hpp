/*
 * TestHelpers.hpp
 *
 */
#ifndef INCLUDE_TESTHELPERS_PERSISTENCY_TESTHELPERS_HPP_FILE
#define INCLUDE_TESTHELPERS_PERSISTENCY_TESTHELPERS_HPP_FILE

/* public header */

#include "Persistency/Alert.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Certainty.hpp"
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


namespace TestHelpers
{
namespace Persistency
{

::Persistency::AlertPtr makeNewAlert(const char *name="some alert");

::Persistency::MetaAlertPtr makeNewMetaAlert(const char *name="some meta-alert");

::Persistency::AnalyzerPtrNN makeNewAnalyzer(const char *name="some analyzer");

const ::Persistency::Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const ::Persistency::Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                  0xFF, 0xFF, 0xFF, 0xFF,
                                                  0xFF, 0xFF, 0xFF, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                 } };
::Persistency::HostPtr makeNewHost(void);
::Persistency::HostPtr makeNewHost4(const char             *ip,
                                    const ::Persistency::Host::Netmask_v4 *mask,
                                    const char             *os,
                                    bool                    nullRef=false);
::Persistency::HostPtr makeNewHost6(const char             *ip,
                                    const ::Persistency::Host::Netmask_v6 *mask,
                                    const char             *os,
                                    bool                    nullRef=false);

::Persistency::ProcessPtr makeNewProcess(const char *name="process name");

::Persistency::ReferenceURLPtr makeNewReferenceURL(const char *url="http://gnu.org");

::Persistency::GraphNodePtrNN makeNewLeaf(void);
::Persistency::GraphNodePtrNN makeNewNode(void);
::Persistency::GraphNodePtrNN makeNewNode(::Persistency::GraphNodePtrNN child1,
                                          ::Persistency::GraphNodePtrNN child2);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      leaf5
//                leaf3 leaf4
//
::Persistency::GraphNodePtrNN makeNewTree1(void);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      node1
//                leaf3 node1    ...
//                       ...
//
::Persistency::GraphNodePtrNN makeNewTree2(void);

} // namespace Persistency
} // namespace TestHelpers

#endif
