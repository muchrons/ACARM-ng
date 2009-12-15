/*
 * Transaction.cpp
 *
 */
#include <string>
#include <cassert>

#include "Persistency/IO/Transaction.hpp"

using std::string;


namespace Persistency
{
namespace IO
{

Transaction::Transaction(TransactionAPIAutoPtr transaction):
  transaction_( transaction.release() ),
  log_("persistency.io.transaction")
{
  if( transaction_.get()==NULL )
    throw ExceptionNULLParameter(__FILE__, "NULL transaction's API");
  assert( transaction_.get()!=NULL );
  logMsg("transaction object created with proper API");
}

Transaction::~Transaction(void)
{
  // if transaction has not been commited, commit it now
  if( transaction_.get()!=NULL )
  {
    logMsg("transaction has not been commited/rollbacked - doing "
           "automatic rollback upon transaction destruction");
    transaction_->rollback();
  }
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

void Transaction::logMsg(const char *str)
{
  assert(str!=NULL);
  assert( transaction_.get()!=NULL && "logging called for finished transaction");
  const string msg=transaction_->getName() + ": " + str;
  LOGMSG_INFO(log_, msg.c_str() );
}

} // namespace IO
} // namespace Persistency
