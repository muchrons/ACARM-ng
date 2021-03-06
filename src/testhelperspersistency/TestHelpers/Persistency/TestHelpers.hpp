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

::Persistency::AlertPtrNN makeNewAlert(const char   *name="some alert",
                                       const char   *sip=NULL,
                                       const char   *tip=NULL,
                                       const char   *dns="dns.org",
                                       const double  certainty=0.42);

::Persistency::MetaAlertPtrNN makeNewMetaAlert(const char *name="some meta-alert", unsigned int id=42);

::Persistency::AnalyzerPtrNN makeNewAnalyzer(const char *name="some analyzer");

const ::Persistency::Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const ::Persistency::Host::Netmask_v6::bytes_type mask6_bytes=
                                                              {
                                                                {
                                                                  0xFF, 0xFF, 0xFF, 0xFF,
                                                                  0xFF, 0xFF, 0xFF, 0x00,
                                                                  0x00, 0x00, 0x00, 0x00,
                                                                  0x00, 0x00, 0x00, 0x00,
                                                                }
                                                              };
::Persistency::HostPtrNN makeNewHost(const char *ip="1.2.3.4", const char *dns="dns.org");
::Persistency::HostPtrNN makeNewHost4(const char             *ip,
                                      const ::Persistency::Host::Netmask_v4 *mask,
                                      const char             *os,
                                      bool                    nullRef=false,
                                      const char             *dns="dns.org");
::Persistency::HostPtrNN makeNewHost6(const char             *ip,
                                      const ::Persistency::Host::Netmask_v6 *mask,
                                      const char             *os,
                                      bool                    nullRef=false,
                                      const char             *dns="dns.org");

::Persistency::ProcessPtrNN makeNewProcess(const char *name="process name");

::Persistency::ReferenceURLPtrNN makeNewReferenceURL(const char *url="http://gnu.org");

::Persistency::GraphNodePtrNN makeNewLeaf(const char   *sip=NULL,
                                          const char   *tip=NULL,
                                          const bool    dns=true,
                                          const double  certainty=0.42);
::Persistency::GraphNodePtrNN makeNewNode(void);
::Persistency::GraphNodePtrNN makeNewNode(::Persistency::GraphNodePtrNN child1,
                                          ::Persistency::GraphNodePtrNN child2,
                                          unsigned int                  id=42);

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

::Persistency::AlertPtrNN makeNewAlertWithHosts( const char                     *hostSrc1,
                                                 const char                     *hostSrc2,
                                                 const char                     *hostDst1,
                                                 const char                     *hostDst2,
                                                 const ::Persistency::Timestamp &createTime=::Persistency::Timestamp() );

::Persistency::GraphNodePtrNN makeNewLeaf(::Persistency::AlertPtrNN alert, unsigned int id=303u);

} // namespace Persistency
} // namespace TestHelpers

#endif
