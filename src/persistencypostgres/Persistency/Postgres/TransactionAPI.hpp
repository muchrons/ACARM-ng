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

/** \brief transaction operations on PostgreSQL DB.
 */
class TransactionAPI: public Persistency::TransactionAPI
{
public:
  /** \brief begins transaction.
   */
  TransactionAPI(void);
  /** \brief commit transaction.
   */
  virtual void commit(void);
  /** \brief rollback transaction.
   */
  virtual void rollback(void);
}; // class TransactionAPI

} // namespace Postgres
} // namespace Persistency

#endif
