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
                               const std::string    &name):
  Persistency::IO::TransactionAPI(mutex, name)
{
  // TODO
}

void TransactionAPI::commit(void)
{
  // TODO
}

void TransactionAPI::rollback(void)
{
  // TODO
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
