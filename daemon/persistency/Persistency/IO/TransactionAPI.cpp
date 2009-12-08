/*
 * TransactionAPI.cpp
 *
 */
#include "Persistency/IO/TransactionAPI.hpp"


namespace Persistency
{
namespace IO
{

TransactionAPI::~TransactionAPI(void)
{
  // TODO: add logging
}

void TransactionAPI::commit(void)
{
  // TODO: add logging
  commitImpl();
}

void TransactionAPI::rollback(void)
{
  // TODO: add logging
  rollbackImpl();
}

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name):
  lock_(mutex),
  name_(name)
{
  // TODO: add logging
}

} // namespace IO
} // namespace Persistency
