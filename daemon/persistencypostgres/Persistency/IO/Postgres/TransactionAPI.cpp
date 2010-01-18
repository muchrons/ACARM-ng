/*
 * TransactionAPI.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Postgres/TransactionAPI.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name,
                               DBHandlerPtrNN        dbHandler):
  Persistency::IO::TransactionAPI(mutex, name),
  t_( dbHandler->getConnection().get(), name ),
  rollback_(false)
{
}

void TransactionAPI::commitImpl(void)
{
  assert(rollback_==false && "commit() called after rollback()");
  t_.commit();
}

void TransactionAPI::rollbackImpl(void)
{
  assert(rollback_==false && "rollback() called multiple times");
  rollback_=true;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
