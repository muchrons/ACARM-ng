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
  IO::TransactionAPI(mutex, name),
  commitCalls_(0),
  rollbackCalls_(0)
{
  assert(persistencyHandler==42 && "invalid handler value received");
  persistencyHandler=0;
}

void TransactionAPI::commitImpl(void)
{
  ++commitCalls_;
}

void TransactionAPI::rollbackImpl(void)
{
  ++rollbackCalls_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
