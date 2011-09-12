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

BackendFacade::BackendFacade(Persistency::IO::ConnectionPtrNN       conn,
                             const Core::Types::Proc::TypeName     &triggerType,
                             const Core::Types::Proc::InstanceName &triggerName):
  Core::Types::BackendFacade(conn, Core::Types::Proc::CategoryName("trigger"), triggerType, triggerName)
{
}

void BackendFacade::markAsTriggered(Persistency::MetaAlertPtrNN ma)
{
  beginTransaction();
  IO::MetaAlertAutoPtr io=getConnection()->metaAlert(ma, getTransaction() );
  io->markAsTriggered( getType().str() + "_" + getName().str() );   // use "type_name" as an identifier
}

} // namespace Trigger
