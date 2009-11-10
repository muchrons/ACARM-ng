/*
 * Transaction.cpp
 *
 */

#include "Persistency/Transaction.hpp"


namespace Persistency
{

Transaction::Transaction(TAPI transaction):
  transaction_( transaction.release() )
{
}

Transaction::~Transaction(void)
{
  if( transaction_.get()!=NULL )
    transaction_->rollback();
}

void Transaction::commit(void)
{
  transaction_->commit();
  transaction_.reset(NULL);
}

void Transaction::rollback(void)
{
  transaction_->rollback();
  transaction_.reset(NULL);
}

} // namespace Persistency
