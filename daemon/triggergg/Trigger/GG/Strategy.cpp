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
namespace detail
{
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
  Lock lock(mutex_);
  LOGMSG_DEBUG(log_, "sending ping to server");
  reconnectIfNeeded();
  ConnectionAutoPtr conn=conn_;     // take connection
  assert( conn.get()!=NULL );

  if( gg_ping( conn.get()->get() )!=0 )
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
    conn_.reset( new Connection(ggCfg_) );
  assert( conn_.get()!=NULL );
}
} // namespace detail


namespace
{
struct PingThread
{
  explicit PingThread(detail::StrategyIO &s):
    log_("trigger.gg.pingthread"),
    s_(&s)
  {
  }

  void operator()(void)
  {
    LOGMSG_DEBUG(log_, "ping thread started");

    bool quit=false;
    // ping server every 60[s]
    while(!quit)
    {
      try
      {
        assert(s_!=NULL);
        s_->ping();                         // ping
        s_->discardIncommingMessages();     // throw away anything that might have arrived

        // wait a while...
        for(int i=0; i<60; ++i)
        {
          boost::this_thread::interruption_point();
          sleep(1);
        }
      }
      catch(const boost::thread_interrupted &)
      {
        // ok - thread has been interrupted
        LOGMSG_DEBUG(log_, "interruption requested - exiting");
        quit=true;
      }
      catch(const std::exception &ex)
      {
        // hmmm...
        LOGMSG_WARN_S(log_)<<"exception caught: "<<ex.what()
                           <<"; if this is connection-related problem system will automatically reconnect soon";
      }
    } // while(true)

    LOGMSG_DEBUG(log_, "leaving scope gentelly");
  }

private:
  Logger::Node        log_;
  detail::StrategyIO *s_;
}; // struct PingThread
} // unnamed namespace


Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("gg", cfg.getThresholdConfig() ),
  io_(cfg),
  pingThread_( PingThread(io_) )
{
}

void Strategy::triggerImpl(const Node &n)
{
  // prepare data
  stringstream  ss;
  Compose::Summary::append(ss, n);

  io_.send( ss.str() );
}

} // namespace GG
} // namespace Trigger
