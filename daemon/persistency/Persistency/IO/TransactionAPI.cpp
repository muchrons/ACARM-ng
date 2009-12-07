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

TransactionAPI::TransactionAPI(Base::Threads::Mutex &mutex):
  lock_(mutex)
{
}

} // namespace IO
} // namespace Persistency
