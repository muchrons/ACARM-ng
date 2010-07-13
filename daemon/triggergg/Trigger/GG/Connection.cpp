/*
 * Connection.cpp
 *
 */
#include "Base/Threads/Lock.hpp"
#include "Logger/Logger.hpp"
#include "Trigger/GG/Connection.hpp"

using namespace Base::Threads;

namespace Trigger
{
namespace GG
{

namespace
{
struct PingThread
{
  explicit PingThread(Connection &conn):
    conn_(&conn)
  {
  }

  void operator()(void)
  {
    // send ping every 42[s]
    while(true)
    {
      boost::thread::interruption_point();
      usleep(42);
      conn_->ping();
    } // while(true)
  }

private:
  Connection *conn_;
}; // struct PingThread
} // unnamed namespace


//
// NOTE: this class is used from multiple threads, though logger node is NOT
//       thread safe! this works because there is never situation when logging could
//       occure from more threads. keep this in mind when doing any refactoring
//       to this class.
//

Connection::Connection(const AccountConfig &cfg):
  log_("trigger.gg.connection"),
  params_(cfg),
  sess_( connect() ),
  pingBlocksCount_(0),
  pingThread_( TODO )
{
  assert( sess_.get()!=NULL );
}

Connection::~Connection(void)
{
  LOGMSG_INFO(log_, "disconnecting from Gadu-Gadu server");
}

void Connection::ping(void)
{
  Lock lock(mutex_);
  if(pingBlocksCount_>0)
  {
    LOGMSG_DEBUG(log, "pinging disabled - ignoring");
    return;
  }
  // TODO
}

void Connection::stopPings(void)
{
  Lock lock(mutex_);
  ++pingBlocksCount_;
}

void Connection::startPings(void)
{
  Lock lock(mutex_);
  assert(pingBlocksCount>0);
  --pingBlocksCount_;
}

// connection to server
gg_session *Connection::connect(void) const
{
  gg_session *sess=gg_login( &params_.get() );

  // sanity check
  if( sess==NULL )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "NULL structure received");

  // check if state is: connected
  if( sess->state!=GG_STATE_CONNECTED )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "not connected to server");

  // looks like everything's done
  LOGMSG_INFO_S(log_) << "connected as " << sess->uin
                      << " to default Gadu-Gadu server; protocol version is "
                      << sess->protocol_version;
  LOGMSG_DEBUG_S(log_) << "current status is " << sess->status;

  // this is *required* part of login process...
  sendContactsList(sess);

  return sess;
}

// Gadu-Gadu protoocl's "feature" - in order to start ANY communication after connecting
// client must send contacts list to server (here: empty). no comments...
void Connection::sendContactsList(gg_session *sess)
{
  assert(sess!=NULL);
  if( gg_notify(sess_, NULL, 0)!=0 )
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, "sending empty contacts list failed");
}

} // namespace GG
} // namespace Trigger
