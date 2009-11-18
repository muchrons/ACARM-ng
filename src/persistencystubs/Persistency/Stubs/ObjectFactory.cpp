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

AlertPtr ObjectFactory::createAlert(/*TODO: parameters to create it from*/) const
{
  return AlertPtr(new Persistency::Stubs::Alert);
}

} // namespace Stubs
} // namespace Persistency
