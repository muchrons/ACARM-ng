/*
 * BackendProxy.cpp
 *
 */
#include <cassert>

#include "Core/Types/Proc/BackendProxy.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;

namespace Core
{
namespace Types
{
namespace Proc
{

BackendProxy::BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
                           const std::string                &processorName):
  processorName_(processorName),
  conn_(conn)
{
  // transaction is not started here yet - it will be initialized when needed
  // for the first time (not to do begin-rollback, useless traffic)
}

BackendProxy::~BackendProxy(void)
{
  // d-tor required to ensure proper destruction of forward-declared objects
}

void BackendProxy::commitChanges(void)
{
  // if no changes were introduced, just do nothing
  if( transaction_.get()==NULL )
    return;

  transaction_->commit();
}

void BackendProxy::beginTransaction(void)
{
  if( transaction_.get()==NULL )    // new transaction
  {
    TransactionAPIAutoPtr api=conn_->createNewTransaction(
                                "transaction_for_proc_" + processorName_);
    transaction_.reset( new Transaction(api) );
  }
  // if begin has been requested, transaction must always be valid
  transaction_->ensureIsActive();
}

Transaction &BackendProxy::getTransaction(void)
{
  assert( transaction_.get()!=NULL );
  return *transaction_;
}

Persistency::IO::ConnectionPtrNN BackendProxy::getConnection(void)
{
  return conn_;
}

} // namespace Proc
} // namespace Types
} // namespace Core
