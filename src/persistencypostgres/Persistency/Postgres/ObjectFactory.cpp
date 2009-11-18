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

// TODO: add data base handling here

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
                                    const Alert::Timestamp     *detected,
                                    const Alert::Timestamp     &created,
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

} // namespace Postgres
} // namespace Persistency
