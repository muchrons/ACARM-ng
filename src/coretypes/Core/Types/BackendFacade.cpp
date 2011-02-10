/*
 * BackendFacade.cpp
 *
 */
#include <cassert>

#include "Core/Types/BackendFacade.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;

namespace Core
{
namespace Types
{

BackendFacade::CustomIOInterface::~CustomIOInterface(void)
{
}

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                             const std::string                &name):
  name_(name),
  conn_(conn)
{
  // transaction is not started here yet - it will be initialized when needed
  // for the first time (not to do begin-rollback, useless traffic)
}

BackendFacade::~BackendFacade(void)
{
  // d-tor required to ensure proper destruction of forward-declared objects
}

void BackendFacade::commitChanges(void)
{
  // if no changes were introduced, just do nothing
  if( transaction_.get()==NULL )
    return;

  transaction_->commit();
  transaction_.reset();
}

void BackendFacade::performCustomIO(CustomIOInterface &ci)
{
  beginTransaction();
  ci.customAction( getConnection(), getTransaction() );
}

void BackendFacade::beginTransaction(void)
{
  if( transaction_.get()==NULL )    // new transaction
  {
    TransactionAPIAutoPtr api=conn_->createNewTransaction( "transaction_for_" + getName() );
    transaction_.reset( new Transaction(api) );
  }
  // if begin has been requested, transaction must always be valid
  transaction_->ensureIsActive();
}

Transaction &BackendFacade::getTransaction(void)
{
  assert( transaction_.get()!=NULL );
  return *transaction_;
}

Persistency::IO::ConnectionPtrNN BackendFacade::getConnection(void)
{
  return conn_;
}

Persistency::IO::DynamicConfigAutoPtr BackendFacade::createDynamicConfig(Persistency::IO::DynamicConfig::Owner &owner)
{
  return conn_->dynamicConfig(owner, *transaction_);
}
const std::string &BackendFacade::getName(void) const
{
  return name_;
}

} // namespace Types
} // namespace Core
