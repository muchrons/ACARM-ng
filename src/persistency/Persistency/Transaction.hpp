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

class Transaction
{
public:
  typedef std::auto_ptr<TransactionAPI> TAPI;

  explicit Transaction(TAPI transaction);

  ~Transaction(void);

  void commit(void);

  void rollback(void);

private:
  boost::scoped_ptr<TransactionAPI> transaction_;
}; // class Transaction

} // namespace Persistency

#endif
