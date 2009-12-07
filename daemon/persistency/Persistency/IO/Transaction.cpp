/*
 * Transaction.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Transaction.hpp"

// TODO: add logging
// TODO: add transaction name

namespace Persistency
{
namespace IO
{

Transaction::Transaction(TransactionAPIAutoPtr transaction):
  transaction_( transaction.release() )
{
  if( transaction_.get()==NULL )
    throw ExceptionNULLParameter(__FILE__, "transaction is NULL");
  assert( transaction_.get()!=NULL );
}

Transaction::~Transaction(void)
{
  // if transaction has not been commited, commit it now
  if( transaction_.get()!=NULL )
    transaction_->rollback();
}

void Transaction::commit(void)
{
  if(transaction_.get()==NULL)
    return;
  transaction_->commit();
  transaction_.reset(NULL);
}

void Transaction::rollback(void)
{
  if(transaction_.get()==NULL)
    return;
  transaction_->rollback();
  transaction_.reset(NULL);
}

} // namespace IO
} // namespace Persistency
