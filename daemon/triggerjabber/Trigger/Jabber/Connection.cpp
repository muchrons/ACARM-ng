/*
 * Connection.cpp
 *
 */
#include "Logger/Logger.hpp"
#include "Trigger/Jabber/Connection.hpp"

namespace Trigger
{
namespace Jabber
{

Connection::Connection(const AccountConfig &cfg):
  log_("trigger.jabber.connection"),
  cfg_(cfg),
  sess_( connect() )
{
  assert( sess_.get()!=NULL );
}

Connection::~Connection(void)
{
  LOGMSG_INFO(log_, "disconnecting from Jabber server");
}

// connection to server
LmConnection *Connection::connect(void) const
{
  LmConnection *sess=lm_connection_new( cfg_.getServer().c_str() );
  // sanity check
  if( sess==NULL )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "NULL structure received (connection creation failed)");
  // TODO: mem-leak of 'sess' if exception throw. use System::ScopedPtrCustom<> helper
  // check if state is: connected
  if( !lm_connection_open_and_block(sess, NULL) )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not connected to server");
  lm_connection_authenticate_and_block(sess, cfg_.getLogin().c_str(), cfg_.getPassword().c_str(), "acarm-ng", NULL);
  // check user authentivation
  if( !lm_connection_is_authenticated(sess) )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not authenticate");
  // send presence message to jabber server (this is needed in order to receive messages)
  LmMessage *m = lm_message_new_with_sub_type(NULL, LM_MESSAGE_TYPE_PRESENCE, LM_MESSAGE_SUB_TYPE_AVAILABLE);
  // TODO: what if m==NULL?
  const bool ret = lm_connection_send(sess, m, NULL);
  // TODO: mem-leak of 'm' if exception throw. use System::ScopedPtrCustom<> helper
  if(ret==false)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, cfg_.getServer(), ret);
  lm_message_unref(m);
  /* setup the connection to send keep alive messages every 60 seconds */
  lm_connection_set_keep_alive_rate(sess, 60);
  // looks like everything's done
  LOGMSG_INFO_S(log_) << "connected as: " << cfg_.getLogin() << " to Jabber server " << cfg_.getServer();

  return sess;
}

} // namespace Jabber
} // namespace Trigger
