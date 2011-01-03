/*
 * ConnectionBuilder.cpp
 *
 */
#include "Persistency/IO/Stubs/Connection.hpp"
#include "Persistency/IO/Stubs/ConnectionBuilder.hpp"

namespace Persistency
{
namespace IO
{
namespace Stubs
{

ConnectionBuilder::ConnectionBuilder(void):
  name_("stubs"),
  log_("persistency.io.stubs")
{
}

ConnectionBuilder::FactoryPtr ConnectionBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Persistency::IO::Stubs");
  return ConnectionBuilder::FactoryPtr(new Connection);
}

const ConnectionBuilder::FactoryTypeName &ConnectionBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
