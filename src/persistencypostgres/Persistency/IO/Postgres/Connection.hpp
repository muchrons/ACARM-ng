/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE

#include "Logger/Node.hpp"
#include "Persistency/IO/ConnectionHelper.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/Heartbeats.hpp"
#include "Persistency/IO/Postgres/DBHandle.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{
/** \brief helper typedef to make declaraionts shorter. */
typedef IO::ConnectionHelper<DBHandlePtrNN,
                             TransactionAPI,
                             Alert,
                             Host,
                             MetaAlert,
                             DynamicConfig,
                             Restorer,
                             Heartbeats>
                        ConnectionBase;
} // namespace Base


/** \brief implementation of Connection for PostgreSQL.
 */
class Connection: public detail::ConnectionBase
{
public:
  /** \brief create instance.
   *  \param handle data base handle to use.
   */
  explicit Connection(DBHandlePtrNN handle);

private:
  virtual size_t removeEntriesOlderThanImpl(size_t days, Transaction &t);
  virtual void issuePeriodicSystemQueriesImpl(Transaction &t);
  virtual void issuePeriodicUserQueriesImpl(Transaction &t);

  Logger::Node log_;
}; // class Connection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
