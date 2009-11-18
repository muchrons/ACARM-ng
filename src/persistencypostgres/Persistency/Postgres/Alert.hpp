/*
 * Alert.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_ALERT_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_ALERT_HPP_FILE

#include "Persistency/Alert.hpp"

// TODO: implement
// TODO: test
// TODO: comment

namespace Persistency
{
namespace Postgres
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
        const ReportedHosts &targetHosts);
}; // class Alert

} // namespace Postgres
} // namespace Persistency

#endif
