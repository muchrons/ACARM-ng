/*
 * Connection.cpp
 *
 */
#include "Persistency/IO/Stubs/Connection.hpp"


namespace Persistency
{
namespace IO
{
namespace Stubs
{

Connection::Connection(void):
  createTransactionCalls_(0),
  alertCalls_(0),
  hostCalls_(0),
  metaAlertCalls_(0),
  dynamicConfigCalls_(0),
  restorerCalls_(0)
{
}

TransactionAPIAutoPtr Connection::createNewTransactionImpl(Base::Threads::Mutex &/*mutex*/,
                                                           const std::string    &name)
{
  ++createTransactionCalls_;
  return impl_.createNewTransaction(name);
}

AlertAutoPtr Connection::alertImpl(AlertPtrNN alert, Transaction &t)
{
  ++alertCalls_;
  return impl_.alert(alert, t);
}

HostAutoPtr Connection::hostImpl(HostPtrNN host, Transaction &t)
{
  ++hostCalls_;
  return impl_.host(host, t);
}

MetaAlertAutoPtr Connection::metaAlertImpl(MetaAlertPtrNN ma, Transaction &t)
{
  ++metaAlertCalls_;
  return impl_.metaAlert(ma, t);
}

DynamicConfigAutoPtr Connection::dynamicConfigImpl(const DynamicConfig::Owner &owner, Transaction &t)
{
  ++dynamicConfigCalls_;
  return impl_.dynamicConfig(owner, t);
}

RestorerAutoPtr Connection::restorerImpl(Transaction &t)
{
  ++restorerCalls_;
  return impl_.restorer(t);
}

size_t Connection::removeEntriesOlderThanImpl(size_t days, Transaction &t)
{
  ++removeOldCalls_;
  return impl_.removeEntriesOlderThan(days, t);
}

void Connection::issuePeriodicSystemQueriesImpl(Transaction &t)
{
  ++periodicSystemQueries_;
}

void Connection::issuePeriodicUserQueriesImpl(Transaction &t)
{
  ++periodicUserQueries_;
}

HeartbeatsAutoPtr Connection::heartbeatsImpl(const Heartbeats::Owner &owner, Transaction &t)
{
  ++heartbeatsCalls_;
  return impl_.heartbeats(owner, t);
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
