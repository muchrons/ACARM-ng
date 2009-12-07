/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_ALERT_HPP_FILE

/* public header */

#include "Persistency/Alert.hpp"
#include "Persistency/Stubs/Analyzer.hpp"
#include "Persistency/Stubs/Severity.hpp"


namespace Persistency
{
namespace Stubs
{
/** \brief represents alert report got from input.
 */
class Alert: public Persistency::Alert
{
public:
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
  Alert(const Name          &name,
        AnalyzerPtr          analyzer,
        const Timestamp     *detected,
        const Timestamp     &created,
        SeverityPtr          severity,
        Certanity            certanity,
        const std::string   *description,
        const ReportedHosts &sourceHosts,
        const ReportedHosts &targetHosts):
    Persistency::Alert(name,
                       analyzer,
                       detected,
                       created,
                       severity,
                       certanity,
                       description,
                       sourceHosts,
                       targetHosts)
  {
  }

  /** \brief create default, minimal object.
   */
  Alert(void):
    Persistency::Alert("alert name",
                       AnalyzerPtr(new Analyzer),
                       NULL,
                       boost::gregorian::from_simple_string("2009-11-17 12:32:42.542761+01"),
                       SeverityPtr(new Severity),
                       Certanity(42.0),
                       NULL,
                       ReportedHosts(),
                       ReportedHosts() )
  {
  }
}; // class Alert

} // namespace Stubs
} // namespace Persistency

#endif
