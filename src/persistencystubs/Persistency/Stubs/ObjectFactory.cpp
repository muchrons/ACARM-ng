/*
 * ObjectFactory.cpp
 *
 */

#include "Persistency/Stubs/ObjectFactory.hpp"
#include "Persistency/Stubs/TransactionAPI.hpp"
#include "Persistency/Stubs/Alert.hpp"


namespace Persistency
{
namespace Stubs
{

TransactionPtr ObjectFactory::createTransaction(void) const
{
  Persistency::Transaction::TAPI tapi(new TransactionAPI);
  return TransactionPtr( new Transaction(tapi) );
}

AlertPtr ObjectFactory::createAlert(const Name          &name,
                                    AnalyzerPtr          analyzer,
                                    const Timestamp     *detected,
                                    const Timestamp     &created,
                                    SeverityPtr          severity,
                                    Certanity            certanity,
                                    const std::string   *description,
                                    const ReportedHosts &sourceHosts,
                                    const ReportedHosts &targetHosts) const
{
  return AlertPtr( new Persistency::Stubs::Alert(name,
                                                 analyzer,
                                                 detected,
                                                 created,
                                                 severity,
                                                 certanity,
                                                 description,
                                                 sourceHosts,
                                                 targetHosts) );
}

} // namespace Stubs
} // namespace Persistency
