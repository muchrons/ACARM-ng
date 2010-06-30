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
} // unnamed namespace

Connection::~Connection(void)
{
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

} // namespace IO
} // namespace Persistency
