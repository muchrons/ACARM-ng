/*
 * ObjectFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_OBJECTFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_OBJECTFACTORY_HPP_FILE

#include "Persistency/ObjectFactory.hpp"


namespace Persistency
{
namespace Stubs
{

/** \brief factory of persistent objects used in program.
 */
class ObjectFactory: public Persistency::ObjectFactory
{
private:
  typedef Persistency::Alert AlertAPI;
public:
  /** \brief creates new transaction.
   *  \return transaction object.
   */
  virtual TransactionPtr createTransaction(void) const;

  /** \brief creates alert.
   *  \param name        name of an alert (~title).
   *  \param analyzer    analyzer that reported an issue.
   *  \param detected    time when alert has been detected.
   *  \param created     time of creation of this alert.
   *  \param severity    severity of alert reported.
   *  \param certanity   certanity about given report.
   *  \param description textual description, if needed.
   *  \param sourceHosts source hosts (attack came from them).
   *  \param targetHosts targeted hosts.
   */
  virtual AlertPtr createAlert(const AlertAPI::Name          &name,
                               AnalyzerPtr                    analyzer,
                               const Timestamp               *detected,
                               const Timestamp               &created,
                               SeverityPtr                    severity,
                               Certanity                      certanity,
                               const std::string             *description,
                               const AlertAPI::ReportedHosts &sourceHosts,
                               const AlertAPI::ReportedHosts &targetHosts) const;
  /** \brief create analyzer entry.
   *  \param name analyzer's name.
   *  \param host host analyzer's running on.
   */
  virtual AnalyzerPtr createAnalyzer(const Analyzer::Name &name,
                                     HostPtr               host) const;
  /** \brief create host entry of IPv4 address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - can be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   */
  virtual HostPtr createHost(const Host::IPv4              &ip,
                             const Host::Netmask_v4        *mask,
                             const Host::OperatingSystem    os,
                             ReferenceURLPtr                url,
                             const Host::ReportedServices  &services,
                             const Host::ReportedProcesses &processes) const;
  /** \brief create host entry of IPv6 address.
   *  \param ip        ip address.
   *  \param mask      network maks of a given host.
   *  \param os        operating system name.
   *  \param url       reference url for report (optional - cen be NULL).
   *  \param services  service reported on a host.
   *  \param processes processes reported on host.
   */
  virtual HostPtr createHost(const Host::IPv6              &ip,
                             const Host::Netmask_v6        *mask,
                             const Host::OperatingSystem    os,
                             ReferenceURLPtr                url,
                             const Host::ReportedServices  &services,
                             const Host::ReportedProcesses &processes) const;
  /** \brief creates process info from given data.
   *  \param path      path to executable
   *  \param name      process name.
   *  \param md5       hash for given binary, if available.
   *  \param pid       proces number (ID).
   *  \param uid       user ID.
   *  \param username  name of user that called process.
   *  \param arguments arguments to the application when running.
   *  \param url       reference URL.
   */
  virtual ProcessPtr createProcess(const Process::Path     &path,
                                   const Process::Name     &name,
                                   const MD5Sum            *md5,
                                   const pid_t             *pid,
                                   const int               *uid,
                                   const Process::Username &username,
                                   const std::string       *arguments,
                                   ReferenceURLPtr          url) const;
  /** \brief creates an object.
   *  \param name name of url (~description).
   *  \param url  url address itself.
   */
  virtual ReferenceURLPtr createReferenceURL(const ReferenceURL::Name &name,
                                             const ReferenceURL::URL  &url) const;
  /** \brief creates object from given data.
   *  \param name     service name.
   *  \param port     port number.
   *  \param protocol communication protocol.
   *  \param url      reference url of thie service.
   */
  virtual ServicePtr createService(const Service::Name     &name,
                                   const Service::Port     &port,
                                   const Service::Protocol &protocol,
                                   ReferenceURLPtr          url) const;
  /** \brief crates class with a given severity.
   *  \param sl level to assign to this instance.
   */
  virtual SeverityPtr createSeverity(SeverityLevel sl) const;

  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Stubs
} // namespace Persistency

#endif
