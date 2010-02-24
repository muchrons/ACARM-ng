/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_ALERT_HPP_FILE

/* public header */

#include <string>
#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/operators.hpp>

#include "Base/NonEmptyVector.hpp"
#include "Commons/SharedPtrNotNULL.hpp"
#include "Persistency/Timestamp.hpp"
#include "Persistency/Analyzer.hpp"
#include "Persistency/Severity.hpp"
#include "Persistency/Certainty.hpp"
#include "Persistency/Host.hpp"
#include "Persistency/detail/LimitedString.hpp"
#include "Persistency/Exception.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"


namespace Persistency
{

/** \brief represents alert report got from input.
 */
class Alert: private boost::noncopyable,
             public  boost::equality_comparable<Alert>
{
public:
  /** \brief name of the alert (aka: title). */
  typedef detail::LimitedString<256>          Name;
  /** \brief vector of reported hosts. */
  typedef std::vector<HostPtrNN>              ReportedHosts;
  /** \brief vector of analyzers assigned to this alert. */
  typedef Base::NonEmptyVector<AnalyzerPtrNN> SourceAnalyzers;

  /** \brief creates alert.
   *  \param name        name of an alert (~title).
   *  \param analyzers   analyzers that reported this alert.
   *  \param detected    time when alert has been detected.
   *  \param created     time of creation of this alert.
   *  \param severity    severity of alert reported.
   *  \param certanity   certanity about given report.
   *  \param description textual description, if needed.
   *  \param sourceHosts source hosts (attack came from them).
   *  \param targetHosts targeted hosts.
   */
  Alert(const Name          &name,
        SourceAnalyzers      analyzers,
        const Timestamp     *detected,
        const Timestamp     &created,
        Severity             severity,
        Certainty            certanity,
        const std::string   &description,
        const ReportedHosts &sourceHosts,
        const ReportedHosts &targetHosts);

  /** \brief gets name of alert.
   *  \return name of alert.
   */
  const Name &getName(void) const;
  /** \brief get list of analyzers that reported alert.
   *  \return analyzers' data.
   */
  const SourceAnalyzers &getSourceAnalyzers(void) const;
  /** \brief gets time alert has been detected.
   *  \return alert detection time.
   */
  const Timestamp *getDetectionTime(void) const;
  /** \brief gets time this alert has been created in the system.
   *  \return creation time of this alert in the system.
   */
  const Timestamp &getCreationTime(void) const;
  /** \brief gets severity of an alert.
   *  \return sererity of alert.
   */
  const Severity &getSeverity(void) const;
  /** \brief gets certanity of alert.
   *  \return certanity, i.e. probability that given alert is valid/real
   *          (not a mistaken).
   */
  Certainty getCertainty(void) const;
  /** \brief gets description of alert.
   *  \return description of alert.
   *  \note this is equivalent of IDMEF's comments.
   */
  const std::string &getDescription(void) const;
  /** \brief gets vector of source hosts reported in this alert.
   *  \return vector of reported source hosts.
   */
  const ReportedHosts &getReportedSourceHosts(void) const;
  /** \brief gets vector of target hosts reported in this alert.
   *  \return vector of reported target hosts.
   */
  const ReportedHosts &getReportedTargetHosts(void) const;
  /** \brief check if classes are equal.
   *  \param other element to compare with.
   *  \return true if elements are equal, false otherwise.
   */
  bool operator==(const Alert &other) const;

private:
  Name                         name_;
  SourceAnalyzers              analyzers_;
  boost::scoped_ptr<Timestamp> detected_;
  Timestamp                    created_;
  Severity                     severity_;
  Certainty                    certanity_;
  std::string                  description_;
  ReportedHosts                sourceHosts_;
  ReportedHosts                targetHosts_;
}; // class Alert


/** \brief smart pointer to Alert type. */
typedef boost::shared_ptr<Alert>         AlertPtr;

/** \brief smart pointer to ALert type, that is sure not to be NULL. */
typedef Commons::SharedPtrNotNULL<Alert> AlertPtrNN;

} // namespace Persistency

#endif
