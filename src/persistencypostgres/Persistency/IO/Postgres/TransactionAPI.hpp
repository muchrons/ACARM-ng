/*
 * TransactionAPI.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_POSTGRES_IO_TRANSACTIONAPI_HPP_FILE
#define INCLUDE_PERSISTENCY_POSTGRES_IO_TRANSACTIONAPI_HPP_FILE

#include <string>
#include <pqxx/pqxx>

#include "Persistency/IO/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"


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
   *  \param mutex    mutex protecting connection object.
   *  \param name     name of transaction.
   *  \param dbHandle data base handle object.
   */
  TransactionAPI(Base::Threads::Mutex &mutex,
                 const std::string    &name,
                 DBHandlePtrNN         dbHandle);
  /** \brief executes statement on handle.
   *  \param sql SQL statement to be executed.
   *  \return query results.
   */
  template<typename T>
  pqxx::result exec(const T& sql)
  {
    return t_.exec(sql);
  }

private:
  /** \brief transaction acceptance opration's interface.
   */
  virtual void commitImpl(void);
  /** \brief transaction abort interface.
   */
  virtual void rollbackImpl(void);

  pqxx::work t_;
  bool       rollback_;
}; // class TransactionAPI

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
