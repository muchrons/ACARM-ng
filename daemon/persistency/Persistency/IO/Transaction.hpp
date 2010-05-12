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

#include "Logger/Logger.hpp"
#include "Persistency/IO/ExceptionTransactionNotActive.hpp"
#include "Persistency/IO/TransactionAPI.hpp"
#include "Persistency/ExceptionNULLParameter.hpp"

namespace Persistency
{
namespace IO
{

/** \brief transaction logic implementation.
 *
 * class uses TransactionAPI as a backend. this is required to avoid
 * pure-virtual member rollback() call in d-tor (default: rollback).
 */
class Transaction: private boost::noncopyable
{
public:
  /** \brief creates transaction algorith for a given persistency transaction
   *         implementation.
   *  \param transaction base object to be used for implementing transaction.
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
  /** \brief throws exception if transaction has been already commited/rollbacked.
   */
  void ensureIsActive(void) const;
  /** \brief gives access to transaction's API implementation.
   *
   *  this method allows doing transaction-specific operations in backend
   *  implementation context. since it only does make sense to haven
   *  final implementation class, method does dynamic cast automatically.
   *
   *  \return reference to API used by given transaction.
   */
  template<typename T>
  T &getAPI(void)
  {
    return dynamic_cast<T&>(*transaction_);
  }

private:
  bool isActive(void) const;

  boost::scoped_ptr<TransactionAPI> transaction_;
  bool                              isActive_;
  Logger::Node                      log_;
}; // class Transaction

} // namespace IO
} // namespace Persistency

#endif
