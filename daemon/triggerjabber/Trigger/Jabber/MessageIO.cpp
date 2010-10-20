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
  assert(msg.c_str()!=NULL);
  // sending itself
  LmMessage *m = lm_message_new_with_sub_type(receiver.c_str(),
                                              LM_MESSAGE_TYPE_MESSAGE,
                                              LM_MESSAGE_SUB_TYPE_CHAT);
  System::ScopedPtrCustom<LmMessage,lm_message_unref> mPtr(m);
  if(mPtr.get()==NULL)
    throw ExceptionCreatingError(SYSTEM_SAVE_LOCATION, receiver);
  if(lm_message_node_add_child (mPtr.get()->node, "body", msg.c_str()) == NULL)
    throw ExceptionCreatingError(SYSTEM_SAVE_LOCATION, receiver);
  assert( conn_.get()!=NULL );
  const bool ret=lm_connection_send( conn_.get(), mPtr.get(), NULL);
  if(ret==false)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, receiver, ret);
  // all done
  LOGMSG_DEBUG_S(log_)<<"message to user "<<receiver<<" sent";
}

void MessageIO::discardIncommingMessages(void)
{
  assert( conn_.get()!=NULL );
  LOGMSG_DEBUG(log_, "discarding all incomming messages");

  // TODO: comment this code, since it's non obvious at the first sight.
  while(g_main_context_iteration(NULL, FALSE))
  {
    LOGMSG_DEBUG(log_, "incomming message discarded");
  }
  LOGMSG_DEBUG(log_, "all messages have been discarded");
}

} // namespace Jabber
} // namespace Trigger
