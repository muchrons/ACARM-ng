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
void StrategyIO::ping(void)
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
} // namespace detail


namespace
{
struct PingThread
{
  explicit PingThread(detail::StrategyIO &s):
    s_(&s)
  {
  }

  void operator()(void)
  {
    try
    {
      // ping server every 60[s]
      while(true)
      {
        for(int i=0; i<60; ++i)
        {
          boost::this_thread::interruption_point();
          sleep(1);
        }
        assert(s_!=NULL);
        s_->ping();
      } // while(true)
    }
    catch(const boost::thread_interrupted &)
    {
      // ok - thread has been interrupted
    }
  }

private:
  detail::StrategyIO *s_;
}; // struct PingThread
} // unnamed namespace


Strategy::Strategy(const Config &cfg):
  Trigger::Simple::Strategy("gg", cfg.getThresholdConfig() ),
  ggCfg_( cfg.getAccountConfig() ),
  receiver_( cfg.getReceiver() ),
  io_(),
  pingThread_( PingThread(io_) )
{
}

void Strategy::triggerImpl(const Node &n)
{
  Lock lock(io_.mutex_);
  // prepare connection
  detail::StrategyIO::ConnectionAutoPtr conn=io_.conn_; // take ownership
  assert( io_.conn_.get()==NULL );
  if( conn.get()==NULL )                                // create connection, if not available
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
  io_.conn_=conn;
  assert( conn.get()     ==NULL );
  assert( io_.conn_.get()!=NULL );
}

} // namespace GG
} // namespace Trigger
