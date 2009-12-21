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
  isActive_(true),
  log_("persistency.io.transaction")
{
  if( transaction_.get()==NULL )
    throw ExceptionNULLParameter(__FILE__, "NULL transaction's API");
  assert( transaction_.get()!=NULL );
  assert( isActive() );
  logMsg("transaction object created with proper API");
}

Transaction::~Transaction(void)
{
  try
  {
    // if transaction has not been commited, rollback it now
    if( isActive() )
    {
      logMsg("transaction has not been commited/rollbacked - doing "
             "automatic rollback upon transaction destruction");
      rollback();
    }
  }
  catch(const std::exception &ex)
  {
    LOGMSG_ERROR_S(log_)<<transaction_->getName()<<": "
                        <<"rollback() failed in d-tor of transaction: "
                        <<ex.what();
  }
  catch(...)
  {
    LOGMSG_ERROR_S(log_)<<transaction_->getName()<<": "
                        <<"rollback() failed in d-tor of transaction";
  }
}

void Transaction::commit(void)
{
  if( !isActive() )
    return;
  transaction_->commit();
  isActive_=false;
}

void Transaction::rollback(void)
{
  if( !isActive() )
    return;
  transaction_->rollback();
  isActive_=false;
}

void Transaction::ensureIsActive(void) const
{
  if( !isActive() )
  {
    LOGMSG_ERROR_S(log_)<<transaction_->getName()<<": "
                        <<"operation called on inactive transaction";
    throw ExceptionTransactionNotActive(__FILE__,
                                        transaction_->getName().c_str() );
  }
}

bool Transaction::isActive(void) const
{
  assert( transaction_.get()!=NULL );
  return isActive_;
}

void Transaction::logMsg(const char *str)
{
  assert(str!=NULL);
  assert( isActive() && "logging called for finished transaction");
  LOGMSG_INFO_S(log_)<<transaction_->getName()<<": "<<str;
}

} // namespace IO
} // namespace Persistency
