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
  impl_(42)
{
}

TransactionAPIAutoPtr Connection::createNewTransactionImpl(Base::Threads::Mutex &/*mutex*/,
                                                           const std::string    &name)
{
  ++createTransactionCalls_;
  return impl_.createNewTransaction(name);
}

AlertAutoPtr Connection::alertImpl(AlertPtr alert, const Transaction &t)
{
  ++alertCalls_;
  return impl_.alert(alert, t);
}

HostAutoPtr Connection::hostImpl(HostPtr host, const Transaction &t)
{
  ++hostCalls_;
  return impl_.host(host, t);
}

MetaAlertAutoPtr Connection::metaAlertImpl(MetaAlertPtr ma, const Transaction &t)
{
  ++metaAlertCalls_;
  return impl_.metaAlert(ma, t);
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
