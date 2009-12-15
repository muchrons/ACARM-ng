/*
 * Connection.cpp
 *
 */
#include "Persistency/IO/Connection.hpp"

namespace Persistency
{
namespace IO
{

Connection::~Connection(void)
{
}

TransactionAPIAutoPtr Connection::createNewTransaction(const std::string &name)
{
  return createNewTransactionImpl(mutex_, name);
}

AlertAutoPtr Connection::alert(AlertPtrNN alert, const Transaction &t)
{
  return alertImpl(alert, t);
}

HostAutoPtr Connection::host(HostPtrNN host, const Transaction &t)
{
  return hostImpl(host, t);
}

MetaAlertAutoPtr Connection::metaAlert(MetaAlertPtrNN ma, const Transaction &t)
{
  return metaAlertImpl(ma, t);
}

} // namespace IO
} // namespace Persistency
