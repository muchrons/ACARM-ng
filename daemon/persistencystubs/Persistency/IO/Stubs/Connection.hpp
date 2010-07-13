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

namespace Persistency
{
namespace IO
{
namespace Stubs
{
namespace detail
{
/** \brief helper typedef to make names shorter.
 */
class ConnectionImpl: public IO::ConnectionHelper<int,   // connection handle - anything...
                                                  Stubs::TransactionAPI,
                                                  Stubs::Alert,
                                                  Stubs::Host,
                                                  Stubs::MetaAlert,
                                                  Stubs::DynamicConfig,
                                                  Stubs::Restorer>
{
public:
  ConnectionImpl(void):
    IO::ConnectionHelper<int,
                         Stubs::TransactionAPI,
                         Stubs::Alert,
                         Stubs::Host,
                         Stubs::MetaAlert,
                         Stubs::DynamicConfig,
                         Stubs::Restorer>(42)
  {
  }
private:
  virtual size_t removeEntriesOlderThanImpl(size_t /*days*/, Transaction &/*t*/)
  {
    return 0;
  }
};
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

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name);
  virtual AlertAutoPtr alertImpl(AlertPtrNN alert, Transaction &t);
  virtual HostAutoPtr hostImpl(HostPtrNN host, Transaction &t);
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtrNN ma, Transaction &t);
  virtual DynamicConfigAutoPtr dynamicConfigImpl(const DynamicConfig::Owner &owner, Transaction &t);
  virtual RestorerAutoPtr restorerImpl(Transaction &t);
  virtual size_t removeEntriesOlderThanImpl(size_t days, Transaction &t);

  detail::ConnectionImpl impl_;
}; // class Connection

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
