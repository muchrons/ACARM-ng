/*
 * Transaction.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_TRANSACTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_TRANSACTION_HPP_FILE

/* public header */

#include <memory>
#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/noncopyable.hpp>

#include "Base/Threads/Mutex.hpp"
#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Persistency/IO/ExceptionTransactionNotActive.hpp"
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
   *  \param mutex connection's mutex to lock.
   *  \param name  name of the transaction.
   */
  Transaction(Base::Threads::Mutex &mutex, const std::string &name);
  /** \brief ends transaction.
   *  \note if transaction was not commited it is automatically rollbacked.
   */
  virtual ~Transaction(void);
  /** \brief performs commit, if operation not done already.
   */
  void commit(void);
  /** \brief rollbacks changes since transaction start.
   */
  void rollback(void);
  /** \brief gets transaciton name.
   *  \return name of this transaction.
   */
  const std::string getName(void) const;
  /** \brief throws exception if transaction has been already commited/rollbacked.
   */
  void ensureIsActive(void) const;

private:
  bool isActive(void) const;
  void logMsg(const char *str);

  virtual void commitImpl(void) = 0;
  virtual void rollbackImpl(void) = 0;

  Base::Threads::Lock lock_;
  const std::string   name_;
  bool                isActive_;
  Logger::Node        log_;
}; // class Transaction


/** \brief auto pointer to IO::Transaction class. */
typedef std::auto_ptr<Transaction> TransactionAutoPtr;

} // namespace IO
} // namespace Persistency

#endif
