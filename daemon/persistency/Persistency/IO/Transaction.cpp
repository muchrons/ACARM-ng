/*
 * Transaction.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Transaction.hpp"

using std::string;


namespace Persistency
{
namespace IO
{

Transaction::Transaction(Base::Threads::Mutex &mutex,
                         const std::string    &name):

  lock_(mutex),
  name_(name),
  isActive_(true),
  log_("persistency.io.transaction")
{
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
    LOGMSG_ERROR_S(log_)<<getName()
                        <<": rollback() failed in d-tor of transaction: "
                        <<ex.what();
  }
  catch(...)
  {
    LOGMSG_ERROR_S(log_)<<getName()
                        <<": rollback() failed in d-tor of transaction";
  }
}

void Transaction::commit(void)
{
  if( !isActive() )
    return;
  commitImpl();
  isActive_=false;
}

void Transaction::rollback(void)
{
  if( !isActive() )
    return;
  rollbackImpl();
  isActive_=false;
}

const std::string Transaction::getName(void) const
{
  return name_;
}

void Transaction::ensureIsActive(void) const
{
  if( !isActive() )
  {
    LOGMSG_ERROR_S(log_)<<getName()
                        <<": operation called on inactive transaction";
    throw ExceptionTransactionNotActive(__FILE__, getName().c_str() );
  }
}

bool Transaction::isActive(void) const
{
  return isActive_;
}

void Transaction::logMsg(const char *str)
{
  assert(str!=NULL);
  assert( isActive() && "logging called for finished transaction");
  LOGMSG_INFO_S(log_)<<getName()<<": "<<str;
}

} // namespace IO
} // namespace Persistency
