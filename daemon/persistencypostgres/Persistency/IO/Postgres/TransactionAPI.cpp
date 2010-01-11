/*
 * TransactionAPI.cpp
 *
 */
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
  dbHandler_(dbHandler)
{
  // TODO
}

void TransactionAPI::commitImpl(void)
{
  // TODO
}

void TransactionAPI::rollbackImpl(void)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
