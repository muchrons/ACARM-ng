/*
 * Connection.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Connection.hpp"

namespace Persistency
{
namespace IO
{

namespace
{
template<typename T>
std::auto_ptr<T> getNonNullAutoPtr(std::auto_ptr<T> ptr)
{
  assert( ptr.get()!=NULL && "call returned NULL pointer unexpectedly");
  return ptr;
} // getNonNullAutoPtr()

bool isLocked(::Base::Threads::Mutex &m)
{
  const bool locked=m.try_lock();
  if(locked)
    m.unlock();
  return !locked;
} // isLocked()
} // unnamed namespace

Connection::~Connection(void)
{
  assert( isLocked(mutex_)==false && "connection is being destroyed while some transaction is still open" );
}

TransactionAPIAutoPtr Connection::createNewTransaction(const std::string &name)
{
  return getNonNullAutoPtr( createNewTransactionImpl(mutex_, name) );
}

AlertAutoPtr Connection::alert(AlertPtrNN alert, Transaction &t)
{
  return getNonNullAutoPtr( alertImpl(alert, t) );
}

HostAutoPtr Connection::host(HostPtrNN host, Transaction &t)
{
  return getNonNullAutoPtr( hostImpl(host, t) );
}

MetaAlertAutoPtr Connection::metaAlert(MetaAlertPtrNN ma, Transaction &t)
{
  return getNonNullAutoPtr( metaAlertImpl(ma, t) );
}

DynamicConfigAutoPtr Connection::dynamicConfig(const DynamicConfig::Owner &owner, Transaction &t)
{
  return getNonNullAutoPtr( dynamicConfigImpl(owner, t) );
}

RestorerAutoPtr Connection::restorer(Transaction &t)
{
  return getNonNullAutoPtr( restorerImpl(t) );
}

size_t Connection::removeEntriesOlderThan(size_t days, Transaction &t)
{
  return removeEntriesOlderThanImpl(days, t);
}

void Connection::issuePeriodicQueries(Transaction &t)
{
  issuePeriodicSystemQueriesImpl(t);
  issuePeriodicUserQueriesImpl(t);
}

HeartbeatsAutoPtr Connection::heartbeats(const Heartbeats::Owner &owner, Transaction &t)
{
  return getNonNullAutoPtr( heartbeatsImpl(owner, t) );
}

} // namespace IO
} // namespace Persistency
