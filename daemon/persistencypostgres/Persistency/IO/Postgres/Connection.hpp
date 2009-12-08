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

/** \brief Connection to the postgresql data base
 */
class Connection: public IO::Connection
{
public:
  Connection(const std::string &server,
             const std::string &dbname,
             const std::string &user,
             const std::string &pass);

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(
                                                  Base::Threads::Mutex &mutex,
                                                  const std::string    &name);
  virtual AlertAutoPtr alertImpl(AlertPtr           alert,
                                 const Transaction &t);
  virtual GraphAutoPtr graphImpl(const Transaction &t);
  virtual HostAutoPtr hostImpl(HostPtr            host,
                               const Transaction &t);
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtr       ma,
                                         const Transaction &t);

  Base::Threads::Mutex mutex_;
}; // class Connection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
