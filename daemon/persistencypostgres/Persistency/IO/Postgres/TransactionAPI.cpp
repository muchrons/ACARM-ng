/*
 * TransactionAPI.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

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
  TRYCATCH_BEGIN
    assert(rollback_==false && "commit() called after rollback()");
    t_.commit();
  TRYCATCH_END
}

void TransactionAPI::rollbackImpl(void)
{
  TRYCATCH_BEGIN
    assert(rollback_==false && "rollback() called multiple times");
    rollback_=true;
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
