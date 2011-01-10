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
  sess_( connect() )
{
  assert( sess_.get()!=NULL );
  sendContactsList();
}

Connection::~Connection(void)
{
  LOGMSG_INFO(log_, "disconnecting from Gadu-Gadu server");
}

// connection to server
AutoSession Connection::connect(void) const
{
  AutoSession sess( gg_login( &params_.get() ) );

  // sanity check
  if( sess.get()==NULL )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "NULL structure received (login failed)");

  // check if state is: connected
  if( sess.get()->state!=GG_STATE_CONNECTED )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not connected to server");

  // looks like everything's done
  LOGMSG_INFO_S(log_) << "connected as " << sess.get()->uin
                      << " to default Gadu-Gadu server; protocol version is "
                      << sess.get()->protocol_version;
  LOGMSG_DEBUG_S(log_) << "current status is " << sess.get()->status;

  return sess;
}

// Gadu-Gadu protoocl's "feature" - in order to start ANY communication after connecting
// client must send contacts list to server (here: empty). no comments...
void Connection::sendContactsList(void)
{
  assert( sess_.get()!=NULL );
  const int ret=gg_notify( sess_.get(), NULL, 0 );
  if(ret!=0)
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "sending empty contacts list failed");
}

} // namespace GG
} // namespace Trigger
