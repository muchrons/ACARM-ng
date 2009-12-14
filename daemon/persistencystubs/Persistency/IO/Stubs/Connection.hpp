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
typedef IO::ConnectionHelper<int,   // connection handler - anything...
                             Stubs::TransactionAPI,
                             Stubs::Alert,
                             Stubs::Host,
                             Stubs::MetaAlert> ConnectionImpl;
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

private:
  virtual TransactionAPIAutoPtr createNewTransactionImpl(Base::Threads::Mutex &mutex,
                                                         const std::string    &name);
  virtual AlertAutoPtr alertImpl(AlertPtr alert, const Transaction &t);
  virtual HostAutoPtr hostImpl(HostPtr host, const Transaction &t);
  virtual MetaAlertAutoPtr metaAlertImpl(MetaAlertPtr ma, const Transaction &t);

  detail::ConnectionImpl impl_;
}; // class Connection

} // namespace Stubs
} // namespace IO
} // namespace Persistency

#endif
