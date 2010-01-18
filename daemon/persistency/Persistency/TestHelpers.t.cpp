/*
 * TestHelpers.t.cpp
 *
 */
#include "Persistency/TestHelpers.t.hpp"

namespace Persistency
{

AnalyzerPtrNN makeNewAnalyzer(const char *name)
{
  return AnalyzerPtrNN( new Analyzer(name, makeNewHost() ) );
}

AnalyzerPtrNN makeNewAnalyzer(const char *name, HostPtr host)
{
  return AnalyzerPtrNN( new Analyzer(name, host) );
}

AlertPtrNN makeNewAlert(void)
{
  return AlertPtrNN( new Persistency::Alert("abc",
                                            makeNewAnalyzer(),
                                            NULL,
                                            Timestamp(),
                                            Severity(SeverityLevel::INFO),
                                            Certanity(0.42),
                                            "some description",
                                            Persistency::Alert::ReportedHosts(),
                                            Persistency::Alert::ReportedHosts()) );
}

HostPtrNN makeNewHost(void)
{
  const Host::Netmask_v4 mask(mask4_bytes);
  return makeNewHost4("1.2.3.4", &mask, "linux");
}

HostPtrNN makeNewHost4(const char             *ip,
                       const Host::Netmask_v4 *mask,
                       const char             *os,
                       bool                    nullRef)
{
  ReferenceURLPtr ref;
  if(!nullRef)
    ref=makeNewReferenceURL();

  return HostPtrNN( new Host( Host::IPv4::from_string(ip),
                              mask,
                              os,
                              ref,
                              Host::ReportedServices(),
                              Host::ReportedProcesses() ) );
}

HostPtrNN makeNewHost6(const char             *ip,
                       const Host::Netmask_v6 *mask,
                       const char             *os,
                       bool                    nullRef)
{
  ReferenceURLPtr ref;
  if(!nullRef)
    ref=makeNewReferenceURL();

  return HostPtrNN( new Host( Host::IPv6::from_string(ip),
                              mask,
                              os,
                              ref,
                              Host::ReportedServices(),
                              Host::ReportedProcesses() ) );
}

ProcessPtrNN makeNewProcess(const char *name)
{
  return ProcessPtrNN( new Process( "/path/to/bin",
                                   name,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL,
                                   ReferenceURLPtr() ) );
}

ReferenceURLPtrNN makeNewReferenceURL(const char *url)
{
  return ReferenceURLPtrNN( new ReferenceURL("some name", url) );
}

} // namespace Persistency
