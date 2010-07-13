/*
 * Strategy.cpp
 *
 */
#include <cassert>

#include "Base/Threads/Lock.hpp"
#include "Trigger/Compose/Summary.hpp"
#include "Trigger/GG/Strategy.hpp"
#include "Trigger/GG/MessageIO.hpp"

using namespace std;
using namespace Base::Threads;

namespace Trigger
{
namespace GG
{

namespace
{
struct PingThread
{
  explicit PingThread(Strategy &s):
    s_(&s)
  {
  }

  void operator()(void)
  {
    // ping server every 60[s]
    while(true)
    {
      sleep(60);
      boost::this_thread::interruption_point();
      assert(s_!=NULL);
      s_->ping();
    } // while(true)
  }

private:
  Strategy *s_;
}; // struct PingThread
} // unnamed namespace


Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("gg", cfg.getThresholdConfig() ),
  ggCfg_( cfg.getAccountConfig() ),
  receiver_( cfg.getReceiver() ),
  pingThread_( PingThread(*this) )   // TODO: passing 'this' of object not yet created...
{
}

void Strategy::ping(void)
{
  Lock lock(mutex_);
  LOGMSG_DEBUG(log_, "sending ping to server");
  if( conn_.get()==NULL )
  {
    LOGMSG_DEBUG(log_, "not connected - ping not send");
    return;
  }
  if( gg_ping( conn_.get()->get() )!=0 )
  {
    LOGMSG_DEBUG(log_, "unable to send ping - unknown error while sending");
    return;
  }
  LOGMSG_DEBUG(log_, "ping sent");
}

void Strategy::triggerImpl(const Node &n)
{
  Lock lock(mutex_);
  // prepare connection
  ConnectionAutoPtr conn=conn_; // take ownership
  assert( conn_.get()==NULL );
  if( conn.get()==NULL )        // create connection, if not available
    conn.reset( new Connection(ggCfg_) );
  assert( conn.get()!=NULL );

  // prepare data
  stringstream  ss;
  Compose::Summary::append(ss, n);

  // send message
  assert( conn.get()!=NULL );
  MessageIO ms( *conn.get() );
  ms.send(receiver_, ss.str() );

  // save opened connection for later usage
  conn_=conn;
  assert( conn.get() ==NULL );
  assert( conn_.get()!=NULL );
}

} // namespace GG
} // namespace Trigger
