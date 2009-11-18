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


  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Persistency

#endif
