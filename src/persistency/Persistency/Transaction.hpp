/*
 * Transaction.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TRANSACTION_HPP_FILE
#define INCLUDE_PERSISTENCY_TRANSACTION_HPP_FILE

/* public header */

#include <memory>
#include <boost/scoped_ptr.hpp>

#include "Persistency/TransactionAPI.hpp"

namespace Persistency
{

/** \brief transaction logic implementation.
 */
class Transaction
{
public:
  /** \brief transaction part, dependent on actual persistency backend.
   */
  typedef std::auto_ptr<TransactionAPI> TAPI;
  /** \brief creates transaction algorith for a given persistency transaction
   *         implementation.
   *  \param transaction base object to be used for implementing transaction
   *                     or NULL, if no transaction is required.
   */
  explicit Transaction(TAPI transaction);
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


/** \brief auto pointer to transaction.
 */
typedef std::auto_ptr<Transaction> TransactionAutoPtr;

} // namespace Persistency

#endif
