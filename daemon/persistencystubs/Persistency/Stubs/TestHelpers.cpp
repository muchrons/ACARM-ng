/*
 * TestHelpers.cpp
 *
 */
#include "Persistency/Stubs/TestHelpers.hpp"

namespace Persistency
{
namespace Stubs
{

AlertPtr makeNewAlert(const char *name)
{
  return AlertPtr( new Alert(name,
                             makeNewAnalyzer(),
                             NULL,
                             Timestamp(),
                             Severity(SeverityLevel::INFO),
                             Certanity(4.2),
                             NULL,
                             Alert::ReportedHosts(),
                             Alert::ReportedHosts() ) );
}

AnalyzerPtr makeNewAnalyzer(const char *name)
{
  return AnalyzerPtr( new Analyzer(name, makeNewHost() ) );
}

AnalyzerPtr makeNewAnalyzer(const char *name, HostPtr host)
{
  return AnalyzerPtr( new Analyzer(name, host) );
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

} // namespace Stubs
} // namespace Persistency
