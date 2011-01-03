/*
 * ConnectionBuilder.cpp
 *
 */
#include <cassert>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/IO/Stubs/Connection.hpp"
#include "Persistency/IO/Stubs/ConnectionBuilder.hpp"

using namespace std;


namespace Persistency
{
namespace IO
{
namespace Stubs
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<BackendFactory, ConnectionBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Persistency_IO_Stubs_ConnectionBuilder)


ConnectionBuilder::ConnectionBuilder(void):
  name_("stubs"),
  log_("persistency.io.stubs")
{
}

ConnectionBuilder::FactoryPtr ConnectionBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Persistency::IO::Stubs");
  assert(g_rh.isRegistered() && "oops - registration failed");

  return ConnectionBuilder::FactoryPtr(new Connection);
}

const ConnectionBuilder::FactoryTypeName &ConnectionBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Stubs
} // namespace IO
} // namespace Persistency
