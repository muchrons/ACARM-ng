/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_TRANSACTIONAPI_HPP_FILE

/* public header */

#include "Persistency/TransactionAPI.hpp"

namespace Persistency
{
namespace Postgres
{

class TransactionAPI: public Persistency::TransactionAPI
{
public:
  TransactionAPI(void);

  virtual void commit(void);

  virtual void rollback(void);
}; // class TransactionAPI

} // namespace Postgres
} // namespace Persistency

#endif
