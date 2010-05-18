/*
 * Host.cpp
 *
 */
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
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
  EntrySaver es(t, *dbHandler_);
  DataBaseID hostID = dbHandler_->getIDCache()->get( get() );
  LOGMSG_DEBUG_S(log_)<<"set name "<<name.get()<<" for Host with ID: "<<hostID;
  es.setHostName(hostID, name);
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
