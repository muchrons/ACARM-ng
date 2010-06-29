/*
 * DynamicConfig.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Persistency/IO/Postgres/DynamicConfig.hpp"
#include "Persistency/IO/Postgres/detail/EntrySaver.hpp"
#include "Persistency/IO/Postgres/detail/EntryReader.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

// this is helper macro for calling f-cjtion that saves line number and calls given sql statement (with log)
#define SQL(sql, log) SQLHelper(__FILE__, __LINE__, (sql), (log))


DynamicConfig::DynamicConfig(const Owner                  &owner,
                             Persistency::IO::Transaction &t,
                             DBHandlerPtrNN                dbHandler):
  Persistency::IO::DynamicConfig(owner, t),
  log_("persisntecy.io.postgres.dynamicconfig"),
  dbHandler_(dbHandler)
{
  LOGMSG_DEBUG_S(log_)<<"creating dynamic configuration for owner: "<<owner.get();
}

void DynamicConfig::writeImpl(Persistency::IO::Transaction &t, const Key &key, const Value &value)
{
  TRYCATCH_BEGIN
    detail::EntrySaver es(t, *dbHandler_);
    es.saveConfigParameter( getOwner(), key, value );
  TRYCATCH_END
}

DynamicConfig::ValueNULL DynamicConfig::readImpl(Persistency::IO::Transaction &t, const Key &key)
{
  TRYCATCH_BEGIN
    detail::EntryReader er(t, *dbHandler_);
    return er.readConfigParameter( getOwner(), key );
  TRYCATCH_END
}

DynamicConfig::Value DynamicConfig::readConstImpl(Persistency::IO::Transaction &t, const Key &key)
{
  TRYCATCH_BEGIN
    detail::EntryReader er(t, *dbHandler_);
    return er.readConstConfigParameter( getOwner(), key );
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
