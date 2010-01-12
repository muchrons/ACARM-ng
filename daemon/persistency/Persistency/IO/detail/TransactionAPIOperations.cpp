/*
 * TransactionAPIOperations.cpp
 *
 */

#include "Persistency/IO/detail/TransactionAPIOperations.hpp"

namespace Persistency
{
namespace IO
{
namespace detail
{

TransactionAPIOperations::TransactionAPIOperations(TransactionAPI &tapi):
  tapi_(tapi)
{
}

void TransactionAPIOperations::commit(void)
{
  tapi_.commit();
}

void TransactionAPIOperations::rollback(void)
{
  tapi_.rollback();
}

} // namespace detail
} // namespace IO
} // namespace Persistency
