/*
 * ObjectFactoryBuilder.cpp
 *
 */
#include <sstream>
#include <cassert>

#include "Commons/Factory/RegistratorHelper.hpp"
#include "Persistency/IO/Postgres/Connection.hpp"
#include "Persistency/IO/Postgres/ConnectionBuilder.hpp"
#include "Persistency/IO/Postgres/TryCatchInAPI.hpp"

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

FORCE_LINK_THIS_OBJECT(Persistency_IO_Postgres_ConnectionBuilder)


ConnectionBuilder::ConnectionBuilder(void):
  name_("postgres"),
  log_("persistency.io.postgres"),
  idCache_(new IDCache)
{
}

ConnectionBuilder::FactoryPtr ConnectionBuilder::buildImpl(const Options &options) const
{
//  TRYCATCH_BEGIN
    LOGMSG_INFO(log_, "building Persistency::IO::Postgres");
    assert(g_rh.isRegistered() && "oops - registration failed");

    const DBConnection::Parameters params( getOption(options, "host"),
                                           getOption(options, "port"),
                                           getOption(options, "dbname"),
                                           getOption(options, "user"),
                                           getOption(options, "pass")  );

    // create and return new handle.
    DBHandlePtrNN handle( new DBHandle(params, idCache_) );
    return ConnectionBuilder::FactoryPtr( new Postgres::Connection(handle) );
//  TRYCATCH_END
  assert(!"code never reaches here");
  throw std::logic_error("code never reaches here");
}

const ConnectionBuilder::FactoryTypeName &ConnectionBuilder::getTypeNameImpl(void) const
{
  return name_;
}

const std::string &ConnectionBuilder::getOption(const Options &options,
                                                const char    *name) const
{
  assert(name!=NULL);
  Options::const_iterator it=options.find(name);
  if( it==options.end() )
    throw ExceptionNoSuchOption(SYSTEM_SAVE_LOCATION, name);
  return it->second;
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
