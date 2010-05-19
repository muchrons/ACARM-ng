/*
 * Host.cpp
 *
 */
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

using namespace Persistency::IO::Postgres::detail;

namespace Persistency
{
namespace IO
{
namespace Postgres
{

Host::Host(Persistency::HostPtrNN  host,
           Transaction            &t,
           DBHandlerPtrNN          dbHandler):
  IO::Host(host, t),
  log_("persistency.io.postgres.host"),
  dbHandler_(dbHandler)
{
}

void Host::setNameImpl(Transaction &t, const Persistency::Host::Name &name)
{
  TRYCATCH_BEGIN
    EntrySaver es(t, *dbHandler_);
    DataBaseID hostID = dbHandler_->getIDCache()->get( get() );
    es.setHostName(hostID, name);
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
