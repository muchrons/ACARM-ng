/*
 * TransactionAPI.cpp
 *
 */
#include <cassert>

#include "System/ignore.hpp"
#include "Persistency/IO/Stubs/TransactionAPI.hpp"


namespace Persistency
{
namespace IO
{
namespace Stubs
{

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex,
                               const std::string    &name,
                               int                   persistencyHandle):
  IO::TransactionAPI(mutex, name),
  commitCalls_(0),
  rollbackCalls_(0)
{
  assert(persistencyHandle==42 && "invalid handle value received");
  System::ignore(persistencyHandle);    // this prevents warning nin release mode
  persistencyHandle=0;
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
