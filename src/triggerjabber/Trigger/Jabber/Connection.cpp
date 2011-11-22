/*
 * Connection.cpp
 *
 */
#include <cassert>
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
  LOGMSG_INFO(log_, "disconnecting from Jabber server");
  try
  {
    sess_->recv(1000);
    // set status to unavailable
    sess_.get()->setPresence(gloox::Presence::Unavailable, 100);
    sess_.get()->recv(1000);
  }
  catch(const std::exception &ex)
  {
    LOGMSG_WARN_S(log_)<<"exception caught: "<<ex.what();
  }
  catch(...)
  {
    LOGMSG_WARN_S(log_)<<"??? unknown exception ???";
  }
}

void Connection::login(gloox::Client *client) const
{
  bool quit=false;
  // login
  do
  {
    assert( client!=NULL );
    gloox::ConnectionError ce = client->recv(1000);
    if(ce != gloox::ConnNoError)
    {
      throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "authentication error");
    }
    else
    {
      // check if user is authorized
      if(client->authed())
        quit=true;
    }
  }
  while(!quit);
}
// connection to server
AutoSession Connection::connect(void) const
{
  gloox::JID jid(cfg_.getLogin() + "@" + cfg_.getServer() + "/acarm-ng");
  AutoSession sess(new gloox::Client(jid, cfg_.getPassword()));
  // sanity check
  assert( sess.get()!=NULL );
  // check if connection was established
  if(!sess->connect(false))
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not connected to server");
  // login
  login( sess.get() );
  if( sess->recv() )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "connection error");
  // set status to available
  sess->setPresence(gloox::Presence::Available, 100 );
  return sess;
}

} // namespace Jabber
} // namespace Trigger
