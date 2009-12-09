/*
 * Alert.cpp
 *
 */

#include "Persistency/Postgres/Alert.hpp"

// TODO: implement
// TODO: test

namespace Persistency
{
namespace Postgres
{

Alert::Alert(const Name          &name,
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
  // TODO: create new entry in DB
}

} // namespace Postgres
} // namespace Persistency
