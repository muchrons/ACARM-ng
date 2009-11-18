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
  virtual AlertPtr createAlert(const Name          &name,
                               AnalyzerPtr          analyzer,
                               const Timestamp     *detected,
                               const Timestamp     &created,
                               SeverityPtr          severity,
                               Certanity            certanity,
                               const std::string   *description,
                               const ReportedHosts &sourceHosts,
                               const ReportedHosts &targetHosts) const;

  //
  // TODO: implement creator methods
  //

}; // class ObjectFactory

} // namespace Stubs
} // namespace Persistency

#endif
