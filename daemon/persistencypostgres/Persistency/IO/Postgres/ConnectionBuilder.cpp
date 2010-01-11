/*
 * ObjectFactoryBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"
#include "Persistency/IO/Postgres/ConnectionBuilder.hpp"

using namespace std;


namespace Persistency
{
namespace IO
{
namespace Postgres
{

namespace
{
// this code registers builder to readers factory, making it available for
// further usage, by generic interface.
const Commons::Factory::RegistratorHelper<BackendFactory, ConnectionBuilder> g_rh;
} // unnamed namespace

FORCE_LINK_THIS_OBJECT(Persistency_IO_Postgres_ConnectionBuilder);


ConnectionBuilder::ConnectionBuilder(void):
  name_("postgres"),
  log_("persistency.io.postgres"),
  idCache_(new IDCache)
{
}

ConnectionBuilder::FactoryPtr ConnectionBuilder::buildImpl(const Options &/*options*/) const
{
  LOGMSG_INFO(log_, "building Persistency::IO::Postgres");
  assert(g_rh.isRegistered() && "oops - registration failed");

  // TODO: implement this
  const string server="TODO";
  const string dbname="TODO";
  const string user  ="TODO";
  const string pass  ="TODO";
  DBConnection::Parameters params={server, dbname, user, pass};

  // create and return new handler.
  DBHandlerPtrNN handler(new DBHandler(params, idCache_) );
  return ConnectionBuilder::FactoryPtr( new Postgres::Connection(handler) );
}

const ConnectionBuilder::FactoryTypeName &ConnectionBuilder::getTypeNameImpl(void) const
{
  return name_;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
