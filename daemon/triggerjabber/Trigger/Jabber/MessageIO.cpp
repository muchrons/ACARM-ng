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
  if(m==NULL)
    throw ExceptionCreatingError(SYSTEM_SAVE_LOCATION, receiver);
  lm_message_node_add_child (m->node, "body", msg.c_str()); // TODO: can this call fail?
  assert( conn_.get()!=NULL );
  const bool ret=lm_connection_send( conn_.get(), m, NULL);
  lm_message_unref (m);             // TODO: wrap 'm' with System::ScopedPtrCustom<> instead of manualy
                                    //       deallocate - it's more error prone in case of future changes
  if(ret==false)
    throw ExceptionSendingError(SYSTEM_SAVE_LOCATION, receiver, ret);
  // all done
  LOGMSG_DEBUG_S(log_)<<"message to user "<<receiver<<" sent";
}

void MessageIO::discardIncommingMessages(void)
{
  assert( conn_.get()!=NULL );
  LOGMSG_DEBUG(log_, "discarding all incomming messages");

  // TODO: refacto this code to loop while have messsages. currently it doesn't look
  //       nice and last log in this call will never happen.
  // TODO: comment this code, since it's non obvious at the first sight.
  while(true)
  {
    // TODO: test it for two triggers
    if( g_main_context_iteration(NULL, FALSE) )
    {
      LOGMSG_DEBUG(log_, "incomming message discarded");
      return;
    }
  }
  LOGMSG_DEBUG(log_, "all messages have been discarded");
}

} // namespace Jabber
} // namespace Trigger
