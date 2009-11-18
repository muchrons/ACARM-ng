/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_STUBS_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_STUBS_TRANSACTIONAPI_HPP_FILE

/* public header */

#include "Persistency/TransactionAPI.hpp"


namespace Persistency
{
namespace Stubs
{
/** \brief transaction calls that are persistency-type-dependent.
 *  \note transaction must begin in derived object's c-tor.
 */
class TransactionAPI: public Persistency::TransactionAPI
{
public:
  /** \brief transaction acceptance interface.
   */
  virtual void commit(void)
  {
  }
  /** \brief transaction abort interface.
   */
  virtual void rollback(void)
  {
  }
}; // class TransactionAPI

} // namespace Stubs
} // namespace Persistency

#endif
