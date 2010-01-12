/*
 * TransactionAPIOperations.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_DETAIL_TRANSACTIONAPIOPERATIONS_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_DETAIL_TRANSACTIONAPIOPERATIONS_HPP_FILE

#include <boost/noncopyable.hpp>

#include "Persistency/IO/TransactionAPI.hpp"

namespace Persistency
{
namespace IO
{
namespace detail
{

/** \brief friend-class for TransactionAPI that allows calling commit/rollback
 *         on its instances.
 *
 *  this is a helper class in fact, that allows internal usage of
 *  TransacitonAPI's commit() and rollback() calls, but prohibit them for
 *  derived objects and implementations.
 *
 *  since TransactionAPI is returned to user via Transaction::getAPI<>() call
 *  it would be too easy to break probram strucutre by 'manual' calls to
 *  commit/rollback implementations that are NOT meant to be used alone.
 *
 *  on the other hand it is not possible to use simple 'friend' declaration
 *  with Transaction class, since TransactionAPI code has to be tested as well.
 */
class TransactionAPIOperations: private boost::noncopyable
{
public:
  /** \brief create instance of TransactionAPI operations.
   *  \param tapi transaction API to work with.
   */
  explicit TransactionAPIOperations(TransactionAPI &tapi);
  /** \brief transaction acceptance opration's interface.
   */
  void commit(void);
  /** \brief transaction abort interface.
   */
  void rollback(void);

private:
  TransactionAPI &tapi_;
}; // class TransactionAPI

} // namespace detail
} // namespace IO
} // namespace Persistency

#endif
