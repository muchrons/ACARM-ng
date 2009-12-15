/*
 * TransactionAPI.cpp
 *
 */
#include <string>
#include <cassert>

#include "Persistency/IO/TransactionAPI.hpp"

using std::string;

namespace Persistency
{
namespace IO
{

TransactionAPI::~TransactionAPI(void)
{
  logMsg("transaction is being destroyed");
}

void TransactionAPI::commit(void)
{
  logMsg("commiting transaction");
  commitImpl();
}

void TransactionAPI::rollback(void)
{
  logMsg("transaction rollback requested");
  rollbackImpl();
}

const std::string TransactionAPI::getName(void) const
{
  return name_;
}

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name):
  lock_(mutex),
  name_(name),
  log_("persistency.io.transactionapi")
{
  logMsg("starting transaction");
}

void TransactionAPI::logMsg(const char *msg)
{
  assert(msg!=NULL);
  const string &str=name_ + ": " + msg;
  LOGMSG_INFO(log_, str.c_str() );
}

} // namespace IO
} // namespace Persistency
