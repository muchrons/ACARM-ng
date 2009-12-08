/*
 * TransactionAPI.cpp
 *
 */
#include "Persistency/IO/Stubs/TransactionAPI.hpp"


namespace Persistency
{
namespace IO
{
namespace Stubs
{

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name):
  IO::TransactionAPI(mutex, name)
{
}

void TransactionAPI::commitImpl(void)
{
}

void TransactionAPI::rollbackImpl(void)
{
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
