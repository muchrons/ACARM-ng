/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_TRANSACTIONAPI_HPP_FILE

/* public header */

namespace Persistency
{

class TransactionAPI
{
public:
  virtual ~TransactionAPI(void)
  {
  }

  virtual void commit(void) = 0;

  virtual void rollback(void) = 0;
}; // class TransactionAPI

} // namespace Persistency

#endif
