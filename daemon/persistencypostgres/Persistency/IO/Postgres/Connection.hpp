/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE

#include "Persistency/IO/Connection.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

// TODO: derive this class from IO::ConnectionHelper

/** \brief Connection to the postgresql data base
 */
class Connection: public IO::Connection
{
public:
  /** \brief create new connection to data base.
   *  \param server server name/address.
   *  \param dbname name of data base to connect to.
   *  \param user   user name to login with.
   *  \param pass   password for user.
   */
  Connection(const std::string &server,
             const std::string &dbname,
             const std::string &user,
             const std::string &pass);

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(
                                                  Base::Threads::Mutex &mutex,
                                                  const std::string    &name);
  virtual AlertAutoPtr alertImpl(AlertPtrNN         alert,
                                 const Transaction &t);
  virtual HostAutoPtr hostImpl(HostPtrNN          host,
                               const Transaction &t);
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN     ma,
                                         const Transaction &t);

  Base::Threads::Mutex mutex_;
}; // class Connection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
