/*
 * MessageIO.cpp
 *
 */
#include <sys/select.h>
#include <loudmouth/loudmouth.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Logger/Logger.hpp"
#include "Trigger/Jabber/MessageIO.hpp"

namespace Trigger
{
namespace Jabber
{

MessageIO::MessageIO(Connection &conn):
  log_("trigger.jabber.messageio"),
  conn_(conn)
{
}

void MessageIO::send(const std::string &receiver, const std::string &msg)
{
  LOGMSG_INFO_S(log_)<<"sending message to user "<<receiver;
  const gchar *tmp=reinterpret_cast<const gchar*>( msg.c_str() );
  assert(tmp!=NULL);
  // sending itself
  assert( conn_.get()!=NULL );
  LmMessage *m = lm_message_new_with_sub_type(receiver.c_str(), LM_MESSAGE_TYPE_MESSAGE, LM_MESSAGE_SUB_TYPE_CHAT);
  lm_message_node_add_child (m->node, "body", tmp);
  const bool ret=lm_connection_send( conn_.get(), m, NULL);
  lm_message_unref (m);
  if(ret==false)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, receiver, ret);
  // all done
  LOGMSG_DEBUG_S(log_)<<"message to user "<<receiver<<" sent";
}

void MessageIO::discardIncommingMessages(void)
{
  // TODO
}

} // namespace Jabber
} // namespace Trigger
