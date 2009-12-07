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

AlertAutoPtr Connection::alert(AlertPtr alert, const Transaction &t)
{
  return alertImpl(alert, t);
}

GraphAutoPtr Connection::graph(const Transaction &t)
{
  return graphImpl(t);
}

HostAutoPtr Connection::host(HostPtr host, const Transaction &t)
{
  return hostImpl(host, t);
}

MetaAlertAutoPtr Connection::metaAlert(MetaAlertPtr ma, const Transaction &t)
{
  return metaAlertImpl(ma, t);
}

} // namespace IO
} // namespace Persistency
