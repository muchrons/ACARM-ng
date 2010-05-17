/*
 * Connection.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Trigger/GG/Connection.hpp"

namespace Trigger
{
namespace GG
{

Connection::Connection(const AccountConfig &cfg):
  log_("trigger.gg.connection"),
  params_(cfg),
  sess_( gg_login( &params_.get() ) )
{
  if( sess_.get()==NULL )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "unable to connect/login to server");
  LOGMSG_INFO(log_, "connected to default Gadu-Gadu server");
}

Connection::~Connection(void)
{
  LOGMSG_INFO(log_, "disconnecting from Gadu-Gadu server");
}

} // namespace GG
} // namespace Trigger
