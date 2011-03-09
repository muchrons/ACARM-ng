/*
 * BackendFacade.cpp
 *
 */
#include <cassert>

#include "Trigger/BackendFacade.hpp"
#include "Persistency/IO/Transaction.hpp"
#include "Persistency/IO/Connection.hpp"

using namespace Persistency;
using namespace Persistency::IO;


namespace Trigger
{

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN  conn,
                             const std::string                &triggerName):
  Core::Types::BackendFacade(conn, triggerName)
{
}

void BackendFacade::markAsTriggered(Persistency::MetaAlertPtrNN ma)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->markAsTriggered( getName() );
}

} // namespace Trigger
