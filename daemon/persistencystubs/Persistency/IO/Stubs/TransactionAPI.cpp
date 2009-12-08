/*
 * TransactionAPI.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Stubs/TransactionAPI.hpp"


namespace Persistency
{
namespace IO
{
namespace Stubs
{

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name,
                               int                   persistencyHandler):
  IO::TransactionAPI(mutex, name)
{
  assert(persistencyHandler==42 && "invalid handler value received");
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
