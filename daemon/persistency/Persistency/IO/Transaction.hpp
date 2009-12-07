/*
 * Transaction.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_TRANSACTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_TRANSACTION_HPP_FILE

/* public header */

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Persistency/IO/TransactionAPI.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"

namespace Persistency
{
namespace IO
{

/** \brief transaction logic implementation.
 */
class Transaction: private boost::noncopyable
{
public:
  /** \brief creates transaction algorith for a given persistency transaction
   *         implementation.
   *  \param transaction base object to be used for implementing transaction
   *                     or NULL, if no transaction is required.
   */
  explicit Transaction(TransactionAPIAutoPtr transaction);
  /** \brief ends transaction.
   *  \note if transaction was not commited it is automatically rollbacked.
   */
  ~Transaction(void);
  /** \brief performs commit, if operation not done already.
   */
  void commit(void);
  /** \brief rollbacks changes since transaction start.
   */
  void rollback(void);

private:
  boost::scoped_ptr<TransactionAPI> transaction_;
}; // class Transaction

} // namespace IO
} // namespace Persistency

#endif
