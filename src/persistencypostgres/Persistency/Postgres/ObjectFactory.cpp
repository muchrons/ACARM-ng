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

AlertPtr ObjectFactory::createAlert(/*TODO: parameters to create it from*/) const
{
  return AlertPtr(new Persistency::Postgres::Alert);
}

} // namespace Postgres
} // namespace Persistency
