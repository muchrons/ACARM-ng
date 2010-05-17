/*
 * MessageSender.cpp
 *
 */
#include <libgadu.h>

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
  // const char * -> const UNSIGNED char *
  const unsigned char *tmp=reinterpret_cast<const unsigned char*>( msg.c_str() );
  // sending itself
  const int ret=gg_send_message( conn_.get(), GG_CLASS_MSG, receiver, tmp);
  if(ret==-1)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, receiver, ret);
}

} // namespace GG
} // namespace Trigger
