/*
 * ObjectFactory.cpp
 *
 */

#include "Persistency/Stubs/ObjectFactory.hpp"
#include "Persistency/Stubs/TransactionAPI.hpp"
#include "Persistency/Stubs/Alert.hpp"
#include "Persistency/Stubs/Analyzer.hpp"


namespace Persistency
{
namespace Stubs
{

TransactionPtr ObjectFactory::createTransaction(void) const
{
  Persistency::Transaction::TAPI tapi(new TransactionAPI);
  return TransactionPtr( new Transaction(tapi) );
}


AlertPtr ObjectFactory::createAlert(const AlertAPI::Name          &name,
                                    AnalyzerPtr                              analyzer,
                                    const AlertAPI::Timestamp     *detected,
                                    const AlertAPI::Timestamp     &created,
                                    SeverityPtr                              severity,
                                    Certanity                                certanity,
                                    const std::string                       *description,
                                    const AlertAPI::ReportedHosts &sourceHosts,
                                    const AlertAPI::ReportedHosts &targetHosts) const
{
  return AlertPtr( new Persistency::Stubs::Alert(name,
                                                 analyzer,
                                                 detected,
                                                 created,
                                                 severity,
                                                 certanity,
                                                 description,
                                                 sourceHosts,
                                                 targetHosts) );
}

AnalyzerPtr ObjectFactory::createAnalyzer(const Analyzer::Name &name,
                                          HostPtr               host) const
{
  return AnalyzerPtr( new Stubs::Analyzer(name, host) );
}

HostPtr ObjectFactory::createHost(const Host::IPv4              &ip,
                                  const Host::Netmask_v4        *mask,
                                  const Host::OperatingSystem    os,
                                  ReferenceURLPtr                url,
                                  const Host::ReportedServices  &services,
                                  const Host::ReportedProcesses &processes) const
{
  return HostPtr( new Stubs::Host(ip, mask, os, url, services, processes) );
}

HostPtr ObjectFactory::createHost(const Host::IPv6              &ip,
                                  const Host::Netmask_v6        *mask,
                                  const Host::OperatingSystem    os,
                                  ReferenceURLPtr                url,
                                  const Host::ReportedServices  &services,
                                  const Host::ReportedProcesses &processes) const
{
  return HostPtr( new Stubs::Host(ip, mask, os, url, services, processes) );
}

} // namespace Stubs
} // namespace Persistency
