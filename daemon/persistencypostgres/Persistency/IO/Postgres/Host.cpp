/*
 * Host.cpp
 *
 */
#include "Persistency/IO/Postgres/Host.hpp"
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
  dbHandler_(dbHandler)
{
}

void Host::setNameImpl(Transaction &t, const Persistency::Host::Name &name)
{
  EntrySaver es(t, *dbHandler_);
  DataBaseID hostID = dbHandler_->getIDCache()->get( get() );
  es.setHostName(hostID, name);
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
