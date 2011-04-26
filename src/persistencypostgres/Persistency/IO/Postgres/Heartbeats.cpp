/*
 * Heartbeats.cpp
 *
 */
#include "Persistency/IO/Postgres/Heartbeats.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"

using namespace Persistency::IO::Postgres::detail;


namespace Persistency
{
namespace IO
{
namespace Postgres
{

Heartbeats::Heartbeats(const Owner &owner, Transaction &t, DBHandlePtrNN dbHandle):
  Persistency::IO::Heartbeats(owner, t),
  log_("persistency.io.postgres.heartbeats"),
  dbHandle_(dbHandle)
{
}

void Heartbeats::reportImpl(Transaction &t, const Owner &o, const Module &m, const Timestamp reported, const unsigned int timeout)
{
  TRYCATCH_BEGIN
    EntrySaver       es(t, *dbHandle_);
    LOGMSG_DEBUG_S(log_)<<"got heartbeat from owner '"<<o.get()<<"', module '"<<m.get()<<"' for "
                        <<reported.get()<<"+"<<timeout;
    es.deleteHeartbeat(o, m);
    es.saveHeartbeat(o, m, reported, timeout);
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
