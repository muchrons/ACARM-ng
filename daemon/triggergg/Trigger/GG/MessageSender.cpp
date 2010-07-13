/*
 * MessageSender.cpp
 *
 */
#include <sys/select.h>
#include <libgadu.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Logger/Logger.hpp"
#include "Trigger/GG/MessageSender.hpp"

namespace Trigger
{
namespace GG
{

MessageSender::MessageSender(Connection &conn):
  conn_(conn)
{
}

void MessageSender::send(const UserID receiver, const std::string &msg)
{
  Logger::Node log("trigger.gg.messagesender");
  discardIncommingMessages(log);
  LOGMSG_INFO_S(log)<<"sending message to user "<<receiver;
  // const char * -> const UNSIGNED char *
  const unsigned char *tmp=reinterpret_cast<const unsigned char*>( msg.c_str() );
  assert(tmp!=NULL);
  // sending itself
  assert( conn_.get()!=NULL );
  const int ret=gg_send_message( conn_.get(), GG_CLASS_CHAT, receiver, tmp);
  if(ret==-1)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, receiver, ret);
  // all done
  LOGMSG_DEBUG_S(log)<<"message to user "<<receiver<<" sent";
}

void MessageSender::discardIncommingMessages(Logger::Node log)
{
  LOGMSG_DEBUG(log, "discarding all incomming messages");

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
        LOGMSG_DEBUG(log, "error while receiving message (to be discarded) - ignoring");
        return;

      default:  // OK
        if( !FD_ISSET(conn_.get()->fd, &desc) )
        {
          LOGMSG_DEBUG(log, "something's wrong with descriptor's setting - check this; problem ignored");
          return;
        }
        // read event
        System::ScopedPtrCustom<gg_event, gg_event_free> e( gg_watch_fd( conn_.get() ) );
        LOGMSG_DEBUG(log, "discarding received message...");
        break;
    } // switch( select() )
  } // while(has_messages)
}

} // namespace GG
} // namespace Trigger
