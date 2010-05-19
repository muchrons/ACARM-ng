/*
 * Connection.cpp
 *
 */
#include <cassert>

#include "Persistency/IO/Connection.hpp"
#include "Persistency/IO/TryCatchInAPI.hpp"

namespace Persistency
{
namespace IO
{

Connection::~Connection(void)
{
}

TransactionAPIAutoPtr Connection::createNewTransaction(const std::string &name)
{
  TRYCATCH_BEGIN
    TransactionAPIAutoPtr ptr=createNewTransactionImpl(mutex_, name);
    assert( ptr.get()!=NULL );
    return ptr;
  TRYCATCH_END
}

AlertAutoPtr Connection::alert(AlertPtrNN alert, Transaction &t)
{
  TRYCATCH_BEGIN
    AlertAutoPtr ptr=alertImpl(alert, t);
    assert( ptr.get()!=NULL );
    return ptr;
  TRYCATCH_END
}

HostAutoPtr Connection::host(HostPtrNN host, Transaction &t)
{
  TRYCATCH_BEGIN
    HostAutoPtr ptr=hostImpl(host, t);
    assert( ptr.get()!=NULL );
    return ptr;
  TRYCATCH_END
}

MetaAlertAutoPtr Connection::metaAlert(MetaAlertPtrNN ma, Transaction &t)
{
  TRYCATCH_BEGIN
    MetaAlertAutoPtr ptr=metaAlertImpl(ma, t);
    assert( ptr.get()!=NULL );
    return ptr;
  TRYCATCH_END
}

RestorerAutoPtr Connection::restorer(Transaction &t)
{
  TRYCATCH_BEGIN
    RestorerAutoPtr ptr=restorerImpl(t);
    assert( ptr.get()!=NULL );
    return ptr;
  TRYCATCH_END
}

size_t Connection::removeEntriesOlderThan(size_t days, Transaction &t)
{
  TRYCATCH_BEGIN
    return removeEntriesOlderThanImpl(days, t);
  TRYCATCH_END
}

} // namespace IO
} // namespace Persistency
