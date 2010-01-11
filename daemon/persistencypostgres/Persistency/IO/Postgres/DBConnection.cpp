/*
 * DBConnection.cpp
 *
 */
#include <sstream>

#include "Persistency/IO/Postgres/DBConnection.hpp"
#include "Logger/Logger.hpp"

namespace Persistency
{
namespace IO
{
namespace Postgres
{

DBConnection::DBConnection(const Parameters &params):
  log_( Logger::NodeName("persistency.io.postgres.dbconnection") ),
  conn_( makeConnectionString(params) ),
  dbname_(params.dbname_)
{
  LOGMSG_INFO_S(log_)<<"connected to '"<<dbname_<<"' data base";
}

DBConnection::~DBConnection(void)
{
  LOGMSG_INFO_S(log_)<<"disconnecting from '"<<dbname_<<"' data base";
}

std::string DBConnection::makeConnectionString(const Parameters &params)
{
  std::stringstream ss;
  ss<<"  host='"    <<params.host_
    <<"' dbname='"  <<params.dbname_
    <<"' user='"    <<params.username_
    <<"' password='"<<params.password_
    <<"'";
  return ss.str();
}

} // namespace Postgres
} // namespace IO
} // namespace Persistency
