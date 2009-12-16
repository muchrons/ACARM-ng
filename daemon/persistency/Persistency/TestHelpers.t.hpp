/*
 * TestHelpers.t.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TESTHELPERS_T_HPP_FILE
#define INCLUDE_PERSISTENCY_TESTHELPERS_T_HPP_FILE

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


namespace Persistency
{

const Host::Netmask_v4::bytes_type mask4_bytes={ {0xFF, 0xFF, 0x00, 0x00} };
const Host::Netmask_v6::bytes_type mask6_bytes={ {
                                                  0xFF, 0xFF, 0xFF, 0xFF,
                                                  0xFF, 0xFF, 0xFF, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                  0x00, 0x00, 0x00, 0x00,
                                                 } };

AnalyzerPtrNN makeNewAnalyzer(const char *name="some analyzer");
AnalyzerPtrNN makeNewAnalyzer(const char *name, HostPtr host);

AlertPtrNN makeNewAlert(void);

HostPtrNN makeNewHost(void);
HostPtrNN makeNewHost4(const char             *ip,
                       const Host::Netmask_v4 *mask,
                       const char             *os,
                       bool                    nullRef=false);
HostPtrNN makeNewHost6(const char             *ip,
                       const Host::Netmask_v6 *mask,
                       const char             *os,
                       bool                    nullRef=false);

ProcessPtrNN makeNewProcess(const char *name="process name");

ReferenceURLPtrNN makeNewReferenceURL(const char *url="http://gnu.org");

} // namespace Persistency

#endif
