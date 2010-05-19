/*
 * Host.cpp
 *
 */
#include "Persistency/IO/Postgres/Host.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

using namespace Persistency::IO::Postgres::detail;

// TODO: always write logs about something in method doing it, instead of line before call
//       to this method. logining at the begining of the method ensures that ALL calls will
//       be logged. this makes code easier to write, read and maintain.

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
    LOGMSG_DEBUG_S(log_)<<"set name "<<name.get()<<" for Host with ID: "<<hostID;
    es.setHostName(hostID, name);
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
