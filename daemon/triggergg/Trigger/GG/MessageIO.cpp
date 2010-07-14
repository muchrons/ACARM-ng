/*
 * MessageIO.cpp
 *
 */
#include <sys/select.h>
#include <libgadu.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Logger/Logger.hpp"
#include "Trigger/GG/MessageIO.hpp"

namespace Trigger
{
namespace GG
{

MessageIO::MessageIO(Connection &conn):
  log_("trigger.gg.messageio"),
  conn_(conn)
{
}

void MessageIO::send(const UserID receiver, const std::string &msg)
{
  LOGMSG_INFO_S(log_)<<"sending message to user "<<receiver;
  // const char * -> const UNSIGNED char *
  const unsigned char *tmp=reinterpret_cast<const unsigned char*>( msg.c_str() );
  assert(tmp!=NULL);
  // sending itself
  assert( conn_.get()!=NULL );
  const int ret=gg_send_message( conn_.get(), GG_CLASS_CHAT, receiver, tmp);
  if(ret==-1)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, receiver, ret);
  // all done
  LOGMSG_DEBUG_S(log_)<<"message to user "<<receiver<<" sent";
}

void MessageIO::discardIncommingMessages(void)
{
  LOGMSG_DEBUG(log_, "discarding all incomming messages");

  while(true)
  {
    assert( conn_.get()!=NULL );
    // wait for something
    fd_set desc;
    FD_ZERO(&desc);
    FD_SET(conn_.get()->fd, &desc);

    // note that we assume running on linux here - other implementations
    // can do random changes to timeout
    timeval timeout={0, 0};      // do not block select()
    switch( select(conn_.get()->fd+1, &desc, NULL, NULL, &timeout) )
    {
      case 0:   // timeout - nothing to be done
        return;

      case -1:  // error - error while READING message is not critical, thus can be ignored
        LOGMSG_DEBUG(log_, "error while receiving message (to be discarded)");
        throw ExceptionReceivingError(SYSTEM_SAVE_LOCATION, "select() returned error");

      default:  // OK
        if( !FD_ISSET(conn_.get()->fd, &desc) )
        {
          LOGMSG_DEBUG(log_, "something's wrong with descriptor's setting - check this; problem ignored");
          throw ExceptionReceivingError(SYSTEM_SAVE_LOCATION, "receive descriptor not set");
          return;
        }
        // read event
        System::ScopedPtrCustom<gg_event, gg_event_free> e( gg_watch_fd( conn_.get() ) );
        if( e.get()==NULL )
        {
          LOGMSG_DEBUG(log_, "NULL pointer read - looks like we're done");
          return;
        }
        LOGMSG_DEBUG(log_, "discarding received message...");
        break;
    } // switch( select() )
  } // while(has_messages)

  LOGMSG_DEBUG(log_, "all messages have been discarded");
}

} // namespace GG
} // namespace Trigger
