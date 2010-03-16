/*
 * BackendProxy.cpp
 *
 */
#include <cassert>

#include "Trigger/BackendProxy.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;


namespace Trigger
{

BackendProxy::BackendProxy(Persistency::IO::ConnectionPtrNN  conn,
                           const std::string                &triggerName):
  Core::Types::Proc::BackendProxy(conn, triggerName)
{
}

void BackendProxy::markAsTriggered(Persistency::MetaAlertPtrNN ma)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->markAsTriggered( getName() );
}

} // namespace Trigger
