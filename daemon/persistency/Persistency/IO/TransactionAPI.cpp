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
}

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name):
  lock_(mutex),
  name_(name)
{
}

} // namespace IO
} // namespace Persistency
