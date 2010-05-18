/*
 * MessageSender.cpp
 *
 */
#include <libgadu.h>
#include <cassert>

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

} // namespace GG
} // namespace Trigger
