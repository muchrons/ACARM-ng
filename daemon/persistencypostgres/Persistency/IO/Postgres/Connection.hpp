/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_POSTGRES_CONNECTION_HPP_FILE

#include "Persistency/IO/ConnectionHelper.hpp"
#include "Persistency/IO/Postgres/TransactionAPI.hpp"
#include "Persistency/IO/Postgres/Alert.hpp"
#include "Persistency/IO/Postgres/MetaAlert.hpp"
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/Restorer.hpp"
#include "Persistency/IO/Postgres/DBHandler.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{
namespace detail
{
/** \brief helper typedef to make declaraionts shorter. */
typedef IO::ConnectionHelper<DBHandlerPtrNN,
                             TransactionAPI,
                             Alert,
                             Host,
                             MetaAlert,
                             DynamicConfig,
                             Restorer>      ConnectionBase;
} // namespace Base


/** \brief implementation of Connection for PostgreSQL.
 */
class Connection: public detail::ConnectionBase
{
public:
  /** \brief create instance.
   *  \param handler data base handler to use.
   */
  explicit Connection(DBHandlerPtrNN handler);

private:
  virtual size_t removeEntriesOlderThanImpl(size_t days, Transaction &t);

  void createTemporaryTables(size_t days, Transaction &t) const;
  void removeExtraMetaAlertsEntries(Transaction &t) const;
  void removeReportedServices(Transaction &t) const;
  void removeReportedProcs(Transaction &t) const;
  void removeReportedHosts(Transaction &t) const;
  void removeAnalyzers(Transaction &t) const;
  size_t removeAlerts(Transaction &t) const;
}; // class Connection

} // namespace Postgres
} // namespace IO
} // namespace Persistency

#endif
