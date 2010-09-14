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

namespace
{

static LmHandlerResult handleMessages (LmMessageHandler * /*handler*/,
                                           LmConnection     * /*connection*/,
                                           LmMessage        *m,
                                           gpointer         userData)
{
  if ((lm_message_get_sub_type (m) != LM_MESSAGE_SUB_TYPE_CHAT &&
       lm_message_get_sub_type (m) != LM_MESSAGE_SUB_TYPE_NORMAL)) {

    return LM_HANDLER_RESULT_ALLOW_MORE_HANDLERS;
  }
  return LM_HANDLER_RESULT_REMOVE_MESSAGE;
}

}

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
  LmMessage *m = lm_message_new_with_sub_type(receiver.c_str(),
                                              LM_MESSAGE_TYPE_MESSAGE,
                                              LM_MESSAGE_SUB_TYPE_CHAT);
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
  LOGMSG_DEBUG(log_, "discarding all incomming messages");
  LmMessageHandler *handler = lm_message_handler_new(handleMessages, NULL, NULL);
  lm_connection_register_message_handler(conn_.get(), handler,
                                         LM_MESSAGE_TYPE_MESSAGE,
                                         LM_HANDLER_PRIORITY_NORMAL);

  lm_message_handler_unref (handler);
  // TODO
  while(true)
  {
    assert( conn_.get()!=NULL );
    if(g_main_context_iteration(NULL, FALSE))
      return;
  }
  LOGMSG_DEBUG(log_, "all messages have been discarded");
}

} // namespace Jabber
} // namespace Trigger
