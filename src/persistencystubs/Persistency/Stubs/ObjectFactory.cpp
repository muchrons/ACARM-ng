/*
 * ObjectFactory.cpp
 *
 */

#include "Persistency/Stubs/ObjectFactory.hpp"
#include "Persistency/Stubs/TransactionAPI.hpp"
#include "Persistency/Stubs/Alert.hpp"
#include "Persistency/Stubs/Analyzer.hpp"
#include "Persistency/Stubs/Host.hpp"
#include "Persistency/Stubs/Process.hpp"
#include "Persistency/Stubs/ReferenceURL.hpp"
#include "Persistency/Stubs/Service.hpp"
#include "Persistency/Stubs/Severity.hpp"


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

ProcessPtr ObjectFactory::createProcess(const Process::Path     &path,
                                        const Process::Name     &name,
                                        const MD5Sum            *md5,
                                        const pid_t             *pid,
                                        const int               *uid,
                                        const Process::Username &username,
                                        const std::string       *arguments,
                                        ReferenceURLPtr          url) const
{
  return ProcessPtr( new Stubs::Process(path,
                                        name,
                                        md5,
                                        pid,
                                        uid,
                                        username,
                                        arguments,
                                        url) );
}

ReferenceURLPtr ObjectFactory::createReferenceURL(const ReferenceURL::Name &name,
                                                  const ReferenceURL::URL  &url) const
{
  return ReferenceURLPtr( new Stubs::ReferenceURL(name, url) );
}

ServicePtr ObjectFactory::createService(const Service::Name     &name,
                                        const Service::Port     &port,
                                        const Service::Protocol &protocol,
                                        ReferenceURLPtr          url) const
{
  return ServicePtr( new Stubs::Service(name, port, protocol, url) );
}

SeverityPtr ObjectFactory::createSeverity(SeverityLevel sl) const
{
  return SeverityPtr( new Stubs::Severity(sl) );
}

} // namespace Stubs
} // namespace Persistency
