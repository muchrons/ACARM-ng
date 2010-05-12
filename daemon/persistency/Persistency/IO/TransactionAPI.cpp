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
  LOGMSG_DEBUG_S(log_)<<name_<<": transaction is being destroyed";
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
  LOGMSG_DEBUG_S(log_)<<name_<<": starting transaction";
}

void TransactionAPI::commit(void)
{
  LOGMSG_DEBUG_S(log_)<<name_<<": commiting transaction";
  commitImpl();
}

void TransactionAPI::rollback(void)
{
  LOGMSG_DEBUG_S(log_)<<name_<<": transaction rollback requested";
  rollbackImpl();
}

} // namespace IO
} // namespace Persistency
