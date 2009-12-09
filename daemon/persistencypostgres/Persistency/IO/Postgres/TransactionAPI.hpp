/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_IO_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_IO_TRANSACTIONAPI_HPP_FILE

#include <string>

#include "Persistency/IO/TransactionAPI.hpp"


namespace Persistency
{
namespace IO
{
namespace Postgres
{

/** \brief transaction calls that are persistency-type-dependent.
 *  \note transaction must begin in derived object's c-tor.
 */
class TransactionAPI: public Persistency::IO::TransactionAPI
{
public:
  /** \brief open transaction.
   *  \param mutex mutex protecting connection object.
   *  \param name  name of transaction.
   */
  TransactionAPI(Base::Threads::Mutex &mutex,
                 const std::string    &name);

  /** \brief transaction acceptance opration's interface.
   */
  virtual void commit(void);
  /** \brief transaction abort interface.
   */
  virtual void rollback(void);

private:
  // TODO
}; // class TransactionAPI

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
