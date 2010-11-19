/*
 * Connection.cpp
 *
 */
#include <gloox/message.h>
#include <gloox/client.h>
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
  assert( sess_.get()!=NULL );
  sess_.get()->disconnect();
  LOGMSG_INFO(log_, "disconnecting from Jabber server");
}

// connection to server
AutoSession Connection::connect(void) const
{
  gloox::JID jid(cfg_.getLogin() + "@" + cfg_.getServer() + "/acarm-ng");
  AutoSession sess(new gloox::Client(jid, cfg_.getPassword()));
  if(!sess.get()->connect(false))
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not connected to server");
  bool quit=false;
  while(!quit)
  {
    gloox::ConnectionError ce = sess.get()->recv();
    if(ce == gloox::ConnNoError)
    {
      if(sess.get()->authed())
        quit=true;
    }
    else
    {
      throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "connection error");
    }
  }
  return sess;
}

} // namespace Jabber
} // namespace Trigger
