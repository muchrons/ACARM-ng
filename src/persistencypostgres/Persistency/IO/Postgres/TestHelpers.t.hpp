/*
 * TestHelpers.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_TESTHELPERS_T_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_TESTHELPERS_T_HPP_FILE

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

#include "Persistency/IO/Postgres/DataBaseID.hpp"


namespace Persistency
{
namespace IO
{
namespace Postgres
{

IO::ConnectionPtrNN makeConnection(void);

DataBaseID getID(IO::Transaction &t, const std::string &name);

void removeNodeConnection(const std::string &parentName, const std::string &childName);
size_t getNoOfMetaAlertsInUse();
Persistency::AlertPtrNN makeNewAlert(const char *name="some alert", const Timestamp &t = Timestamp(444) );

Persistency::MetaAlertPtrNN makeNewMetaAlert(const unsigned int id, const char *name="some meta-alert", const Timestamp &t=Timestamp(444));

Persistency::AnalyzerPtrNN makeNewAnalyzer(const char *name="some analyzer");

const Persistency::Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const Persistency::Host::Netmask_v6::bytes_type mask6_bytes=
                                                            {
                                                              {
                                                                0xFF, 0xFF, 0xFF, 0xFF,
                                                                0xFF, 0xFF, 0xFF, 0x00,
                                                                0x00, 0x00, 0x00, 0x00,
                                                                0x00, 0x00, 0x00, 0x00,
                                                              }
                                                            };
Persistency::HostPtrNN makeNewHost(void);
Persistency::HostPtrNN makeNewHostWithNullRefUrl(void);
Persistency::HostPtrNN makeNewHost4(const char             *ip,
                                    const Persistency::Host::Netmask_v4 *mask,
                                    const char             *os,
                                    bool                    nullRef=false);
Persistency::HostPtrNN makeNewHost6(const char             *ip,
                                    const Persistency::Host::Netmask_v6 *mask,
                                    const char             *os,
                                    bool                    nullRef=false);

Persistency::ProcessPtrNN makeNewProcess(const char *name="process name");

Persistency::ReferenceURLPtrNN makeNewReferenceURL(const char *url="http://gnu.org");

Persistency::GraphNodePtrNN makeNewLeaf(const unsigned int  id,
                                        const char         *name="some name",
                                        const Timestamp    &time=Timestamp(444));

Persistency::GraphNodePtrNN makeNewNode(void);

Persistency::GraphNodePtrNN makeNewNode(const unsigned int           id,
                                        Persistency::GraphNodePtrNN  child1,
                                        Persistency::GraphNodePtrNN  child2,
                                        const char                  *name="some node",
                                        const Timestamp             &time=Timestamp(444));

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      leaf5
//                leaf3 leaf4
//
Persistency::GraphNodePtrNN makeNewTree1(void);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3      node1
//                leaf3 node1    ...
//                       ...
//
Persistency::GraphNodePtrNN makeNewTree2(void);
//
//                   root1       root2
//             node1       node2       node3
//       leaf1       leaf2       leaf3       leaf4
Restorer::NodesVector makeNewTree3(void);

//
//                root
//     node1               node2
//  leaf1 leaf2      node3       node4
//                leaf3 leaf4 leaf5 leaf6
//
Persistency::GraphNodePtrNN makeNewTree4(void);
//
//                   root1
//             node1       node2
//       node3       node4       node5
//
Restorer::NodesVector makeNewTree5(void);
//
//     node1         node2
//  leaf1 leaf2   leaf3 leaf4
Restorer::NodesVector makeNewTree6(void);
//
//     node1
//  leaf1  leaf2
Restorer::NodesVector makeNewTree7(void);
} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
