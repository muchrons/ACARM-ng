/*
 * StrategyIO.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Trigger/Compose/Summary.hpp"
#include "Trigger/Jabber/Strategy.hpp"
#include "Trigger/Jabber/MessageIO.hpp"

using namespace std;
using namespace Base::Threads;

namespace Trigger
{
namespace Jabber
{
namespace detail
{

StrategyIO::StrategyIO(const Config &cfg):
  log_("trigger.jabber.detail.strategyio"),
  jabberCfg_( cfg.getAccountConfig() ),
  receivers_( cfg.getReceivers() )
{
  // NOTE: do NOT connect in c-tor - object must be created first and connection
  //       will then be estabilished uppon first usage.
}

void StrategyIO::send(const std::string &msg)
{
  Lock lock(mutex_);
  LOGMSG_DEBUG(log_, "sending message");
  // prepare connection
  reconnectIfNeeded();
  detail::StrategyIO::ConnectionAutoPtr conn=conn_; // take ownership
  assert( conn_.get()==NULL );
  assert( conn.get() !=NULL );

  // send message
  MessageIO ms( *conn.get() );
  int       sent=0;
  for(Config::Receivers::const_iterator it=receivers_.begin(); it!=receivers_.end(); ++it)
  {
    try
    {
      ms.send(*it, msg);
      ++sent;
    }
    catch(const Commons::Exception &ex)
    {
      LOGMSG_ERROR_S(log_)<<"sening message to "<<*it<<" failed; proceeding with next one...";
    }
  }
  // test if any message has been sent at all
  if(sent==0)
    throw Exception(SYSTEM_SAVE_LOCATION, "filed to send messages to all recipient");

  // save opened connection for later usage
  conn_=conn;                                       // given ownership back
  assert( conn.get() ==NULL );
  assert( conn_.get()!=NULL );
}

void StrategyIO::ping(void)
{
  Lock lock(mutex_);
  LOGMSG_DEBUG(log_, "sending ping to server");
  reconnectIfNeeded();
  ConnectionAutoPtr conn=conn_;     // take connection
  assert( conn.get()!=NULL );
  if(conn.get()->get()->recv() != gloox::ConnNoError)
  {
    LOGMSG_DEBUG(log_, "unable to send ping - unknown error while sending");
    return;
  }
  LOGMSG_DEBUG(log_, "ping sent");
  conn_=conn;                       // give connection back
  assert( conn_.get()!=NULL );
}

void StrategyIO::discardIncommingMessages(void)
{
  Lock lock(mutex_);
  LOGMSG_DEBUG(log_, "discarding messages account could have recieved");
  reconnectIfNeeded();
  ConnectionAutoPtr conn=conn_;     // take connection
  assert( conn.get()!=NULL );

  // now discard all messages
  MessageIO mio( *conn.get() );
  mio.discardIncommingMessages();

  LOGMSG_DEBUG(log_, "discarding messages done");
  conn_=conn;                       // give connection back
  assert( conn_.get()!=NULL );
}

void StrategyIO::reconnectIfNeeded(void)
{
  if( conn_.get()==NULL )           // create connection, if not available
    conn_.reset( new Connection(jabberCfg_) );
  assert( conn_.get()!=NULL );
}

} // namespace detail
} // namespace Jabber
} // namespace Trigger
