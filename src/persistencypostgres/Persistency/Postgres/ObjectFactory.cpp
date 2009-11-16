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

ObjectFactory::TransactionAutoPtr ObjectFactory::createTransaction(void) const
{
  Persistency::Transaction::TAPI tapi(new TransactionAPI);
  return TransactionAutoPtr( new Transaction(tapi) );
}

ObjectFactory::AlertAutoPtr ObjectFactory::createAlert(/*TODO: parameters to create it from*/) const
{
  return AlertAutoPtr(new Persistency::Postgres::Alert);
}

} // namespace Postgres
} // namespace Persistency
