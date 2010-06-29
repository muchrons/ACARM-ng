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


DynamicConfig::DynamicConfig(const Owner &owner, Persistency::IO::Transaction &t):
  Persistency::IO::DynamicConfig(owner, t),
  log_("persisntecy.io.postgres.dynamicconfig")
{
  LOGMSG_DEBUG_S(log_)<<"creating dynamic configuration for owner: "<<owner.get();
}

void DynamicConfig::writeImpl(Persistency::IO::Transaction &t, const Key &key, const Value &value)
{
  TRYCATCH_BEGIN
  // TODO
  TRYCATCH_END
}

DynamicConfig::ValueNULL DynamicConfig::readImpl(Persistency::IO::Transaction &t, const Key &key)
{
  TRYCATCH_BEGIN
    // TODO
    return ValueNULL();
  TRYCATCH_END
}

DynamicConfig::Value DynamicConfig::readConstImpl(Persistency::IO::Transaction &t, const Key &key)
{
  TRYCATCH_BEGIN
    // TODO
    throw ExceptionNoSuchParameter(SYSTEM_SAVE_LOCATION, key);
    return "";
  TRYCATCH_END
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
