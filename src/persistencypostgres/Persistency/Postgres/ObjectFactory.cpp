/*
 * ObjectFactory.cpp
 *
 */

#include "Persistency/Postgres/ObjectFactory.hpp"
#include "Persistency/Postgres/TransactionAPI.hpp"
#include "Persistency/Postgres/Alert.hpp"


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
  return AlertPtr( new Persistency::Postgres::Alert(name,
                                                    analyzer,
                                                    detected,
                                                    created,
                                                    severity,
                                                    certanity,
                                                    description,
                                                    sourceHosts,
                                                    targetHosts) );
}

} // namespace Postgres
} // namespace Persistency
