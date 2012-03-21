/*
 * Connection.hpp
 *
 */
#ifndef INCLUDE_PERSISTENCY_IO_STUBS_CONNECTION_HPP_FILE
#define INCLUDE_PERSISTENCY_IO_STUBS_CONNECTION_HPP_FILE

#include "Persistency/IO/ConnectionHelper.hpp"
#include "Persistency/IO/Stubs/TransactionAPI.hpp"
#include "Persistency/IO/Stubs/Alert.hpp"
#include "Persistency/IO/Stubs/Host.hpp"
#include "Persistency/IO/Stubs/MetaAlert.hpp"
#include "Persistency/IO/Stubs/DynamicConfig.hpp"
#include "Persistency/IO/Stubs/Restorer.hpp"
#include "Persistency/IO/Stubs/Heartbeats.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{
namespace detail
{

/** \brief helper declaration of base for connection implementation base. */
typedef IO::ConnectionHelper<int,   // connection handle - can be anything here...
                             Stubs::TransactionAPI,
                             Stubs::Alert,
                             Stubs::Host,
                             Stubs::MetaAlert,
                             Stubs::DynamicConfig,
                             Stubs::Restorer,
                             Stubs::Heartbeats>
                    ConnectionImplBase;

/** \brief helper typedef to make names shorter.
 */
class ConnectionImpl: public ConnectionImplBase
{
public:
  ConnectionImpl(void):
    ConnectionImplBase(42)
  {
  }
private:
  virtual size_t removeEntriesOlderThanImpl(size_t /*days*/, Transaction &/*t*/)
  {
    return 0;
  }
  virtual void issuePeriodicSystemQueriesImpl(Transaction &/*t*/)
  {
  }
  virtual void issuePeriodicUserQueriesImpl(Transaction &/*t*/)
  {
  }

}; // class ConnectionImpl
} // namespace detail


/** \brief stub of connection element
 */
class Connection: public IO::Connection
{
public:
  /** \brief create new connection to nowhere.
   */
  Connection(void);

  size_t createTransactionCalls_;   ///< number of calls to create transaction.
  size_t alertCalls_;               ///< number of calls to create alerts.
  size_t hostCalls_;                ///< number of calls to create hosts.
  size_t metaAlertCalls_;           ///< number of calls to create metaAlerts.
  size_t dynamicConfigCalls_;       ///< number of calls to create dynamicConfig
  size_t restorerCalls_;            ///< number of calls to create restorers.
  size_t removeOldCalls_;           ///< number of calls to removing old entries.
  size_t heartbeatsCalls_;          ///< number of calls to heartbeats.
  size_t periodicSystemQueries_;    ///< number of calls to periodic system queries.
  size_t periodicUserQueries_;      ///< number of calls to periodic user queries.

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name);
  virtual AlertAutoPtr alertImpl(AlertPtrNN alert, Transaction &t);
  virtual HostAutoPtr hostImpl(HostPtrNN host, Transaction &t);
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, Transaction &t);
  virtual DynamicConfigAutoPtr dynamicConfigImpl(const DynamicConfig::Owner &owner, Transaction &t);
  virtual RestorerAutoPtr restorerImpl(Transaction &t);
  virtual size_t removeEntriesOlderThanImpl(size_t days, Transaction &t);
  virtual void issuePeriodicSystemQueriesImpl(Transaction &t);
  virtual void issuePeriodicUserQueriesImpl(Transaction &t);
  virtual HeartbeatsAutoPtr heartbeatsImpl(const Heartbeats::Owner &owner, Transaction &t);

  detail::ConnectionImpl impl_;
}; // class Connection

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
