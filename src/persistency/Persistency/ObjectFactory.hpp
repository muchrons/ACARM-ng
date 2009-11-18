/*
 * ObjectFactory.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_OBJECTFACTORY_HPP_FILE
#define INCLUDE_PERSISTENCY_OBJECTFACTORY_HPP_FILE

/* public header */

#include <boost/noncopyable.hpp>

#include "Persistency/Transaction.hpp"
#include "Persistency/Alert.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Host.hpp"


namespace Persistency
{

/** \brief factory of persistent objects used in program.
 */
class ObjectFactory: private boost::noncopyable
{
public:
  /** \brief ensure proper destruction.
   */
  virtual ~ObjectFactory(void);

  // TODO: add transaction's name (good for logging)
  /** \brief creates new transaction.
   *  \return transaction object.
   */
  virtual TransactionPtr createTransaction(void) const = 0;
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
  virtual AlertPtr createAlert(const Alert::Name          &name,
                               AnalyzerPtr                 analyzer,
                               const Alert::Timestamp     *detected,
                               const Alert::Timestamp     &created,
                               SeverityPtr                 severity,
                               Certanity                   certanity,
                               const std::string          *description,
                               const Alert::ReportedHosts &sourceHosts,
                               const Alert::ReportedHosts &targetHosts) const = 0;
  /** \brief create analyzer entry.
   *  \param name analyzer's name.
   *  \param host host analyzer's running on.
   */
  virtual AnalyzerPtr createAnalyzer(const Analyzer::Name &name,
                                     HostPtr               host) const = 0;
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
                             const Host::ReportedProcesses &processes) const = 0;
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
                             const Host::ReportedProcesses &processes) const = 0;
  /** \brief creates object from given data.
   *  \param path      path to executable
   *  \param name      process name.
   *  \param md5       hash for given binary, if available.
   *  \param pid       proces number (ID).
   *  \param uid       user ID.
   *  \param username  name of user that called process.
   *  \param arguments arguments to the application when running.
   *  \param url       reference URL.
   */
  /*
  virtual ProcessPtr createProcess(const Path        &path,
                                   const Name        &name,
                                   const MD5Sum      *md5,
                                   const pid_t       *pid,
                                   const int         *uid,
                                   const Username    &username,
                                   const std::string *arguments,
                                   ReferenceURLPtr    url) const = 0;
  */



  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Persistency

#endif
