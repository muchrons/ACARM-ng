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
                               AnalyzerPtr                             analyzer,
                               const AlertAPI::Timestamp     *detected,
                               const AlertAPI::Timestamp     &created,
                               SeverityPtr                             severity,
                               Certanity                               certanity,
                               const std::string                      *description,
                               const AlertAPI::ReportedHosts &sourceHosts,
                               const AlertAPI::ReportedHosts &targetHosts) const;
  /** \brief create analyzer entry.
   *  \param name analyzer's name.
   *  \param host host analyzer's running on.
   */
  virtual AnalyzerPtr createAnalyzer(const Analyzer::Name &name,
                                     HostPtr               host) const;

  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Stubs
} // namespace Persistency

#endif
