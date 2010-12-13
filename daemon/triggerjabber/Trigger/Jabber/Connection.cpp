/*
 * Connection.cpp
 *
 */
#include <gloox/message.h>
#include <gloox/client.h>
#include <gloox/presence.h>
#include "Logger/Logger.hpp"
#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Jabber/Connection.hpp"


namespace Trigger
{
namespace Jabber
{
namespace
{
  struct BlockConnTHread
  {
  }; // struct BlockConnThread
} // unnamed namespace

Connection::Connection(const AccountConfig &cfg):
  log_("trigger.jabber.connection"),
  cfg_(cfg),
  sess_( connect() )
{
  assert( sess_.get()!=NULL );
}

Connection::~Connection(void)
{
  sess_.get()->recv(1000);
  // set status to unavailable
  sess_.get()->setPresence(gloox::Presence::Unavailable, 100 );
  LOGMSG_INFO(log_, "disconnecting from Jabber server");
}

// connection to server
AutoSession Connection::connect(void) const
{
  gloox::JID jid(cfg_.getLogin() + "@" + cfg_.getServer() + "/acarm-ng");
  AutoSession sess(new gloox::Client(jid, cfg_.getPassword()));
  // sanity check
  if( sess.get()==NULL )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "NULL structure received (login failed)");
  // check if connection was established
  if(!sess.get()->connect(false))
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not connected to server");
  bool quit=false;
  // login
  while(!quit)
  {
    gloox::ConnectionError ce = sess.get()->recv();
    if(ce == gloox::ConnNoError)
    {
      // check if user is authorized
      if(sess.get()->authed())
        quit=true;
    }
    else
    {
      throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "authentication error");
    }
  }
  if( sess.get()->recv() )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "connection error");
  // set status to available
  sess.get()->setPresence(gloox::Presence::Available, 100 );
  return sess;
}

} // namespace Jabber
} // namespace Trigger
