/*
 * DynamicConfig.cpp
 *
 */
#include "Persistency/IO/Postgres/DynamicConfig.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

DynamicConfig::DynamicConfig(const char *owner, Persistency::IO::Transaction &t):
  Persistency::IO::DynamicConfig(owner, t)
{
}

void DynamicConfig::writeImpl(Persistency::IO::Transaction &/*t*/, const std::string &/*key*/, const std::string &/*value*/)
{
  // TODO
}

DynamicConfig::StringNULL DynamicConfig::readImpl(Persistency::IO::Transaction &/*t*/, const std::string &/*key*/)
{
  // TODO
  return StringNULL();
}

std::string DynamicConfig::readConstImpl(Persistency::IO::Transaction &/*t*/, const std::string &key)
{
  // TODO
  throw ExceptionNoSuchParameter(SYSTEM_SAVE_LOCATION, key);
  return "";
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
