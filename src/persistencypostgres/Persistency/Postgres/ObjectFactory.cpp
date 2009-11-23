/*
 * ObjectFactory.cpp
 *
 */

#include "Persistency/Postgres/ObjectFactory.hpp"
#include "Persistency/Postgres/TransactionAPI.hpp"
#include "Persistency/Postgres/Alert.hpp"
#include "Persistency/Postgres/Analyzer.hpp"
#include "Persistency/Postgres/Host.hpp"
#include "Persistency/Postgres/Process.hpp"
#include "Persistency/Postgres/ReferenceURL.hpp"
#include "Persistency/Postgres/Service.hpp"
#include "Persistency/Postgres/Severity.hpp"


namespace Persistency
{
namespace Postgres
{

TransactionPtr ObjectFactory::createTransaction(void) const
{
  Persistency::Transaction::TAPI tapi(new TransactionAPI);
  return TransactionPtr( new Transaction(tapi) );
}

AlertPtr ObjectFactory::createAlert(const Alert::Name          &name,
                                    AnalyzerPtr                 analyzer,
                                    const Timestamp            *detected,
                                    const Timestamp            &created,
                                    SeverityPtr                 severity,
                                    Certanity                   certanity,
                                    const std::string          *description,
                                    const Alert::ReportedHosts &sourceHosts,
                                    const Alert::ReportedHosts &targetHosts) const
{
  return AlertPtr( new Postgres::Alert(name,
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
  return AnalyzerPtr( new Postgres::Analyzer(name, host) );
}

HostPtr ObjectFactory::createHost(const Host::IPv4              &ip,
                                  const Host::Netmask_v4        *mask,
                                  const Host::OperatingSystem    os,
                                  ReferenceURLPtr                url,
                                  const Host::ReportedServices  &services,
                                  const Host::ReportedProcesses &processes) const
{
  return HostPtr( new Postgres::Host(ip, mask, os, url, services, processes) );
}

HostPtr ObjectFactory::createHost(const Host::IPv6              &ip,
                                  const Host::Netmask_v6        *mask,
                                  const Host::OperatingSystem    os,
                                  ReferenceURLPtr                url,
                                  const Host::ReportedServices  &services,
                                  const Host::ReportedProcesses &processes) const
{
  return HostPtr( new Postgres::Host(ip, mask, os, url, services, processes) );
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
  return ProcessPtr( new Postgres::Process(path,
                                           name,
                                           md5,
                                           pid,
                                           uid,
                                           username,
                                           arguments,
                                           url) );
}

ReferenceURLPtr ObjectFactory::createReferenceURL(
                                        const ReferenceURL::Name &name,
                                        const ReferenceURL::URL  &url) const
{
  return ReferenceURLPtr( new Postgres::ReferenceURL(name, url) );
}

ServicePtr ObjectFactory::createService(const Service::Name     &name,
                                        const Service::Port     &port,
                                        const Service::Protocol &protocol,
                                        ReferenceURLPtr          url) const
{
  return ServicePtr( new Postgres::Service(name, port, protocol, url) );
}

SeverityPtr ObjectFactory::createSeverity(SeverityLevel sl) const
{
  return SeverityPtr( new Postgres::Severity(sl) );
}

} // namespace Postgres
} // namespace Persistency
