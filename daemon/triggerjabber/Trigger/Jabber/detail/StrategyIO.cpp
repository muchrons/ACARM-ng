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
  receiver_( cfg.getReceiver() )
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
  ms.send(receiver_, msg);

  // save opened connection for later usage
  conn_=conn;                                       // given ownership back
  assert( conn.get() ==NULL );
  assert( conn_.get()!=NULL );
}

void StrategyIO::ping(void)
{
 //TODO: consider if ping is needed for jabber connection
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
