/*
 * MessageIO.cpp
 *
 */
#include <sys/select.h>
//#include <loudmouth/loudmouth.h>
#include <gloox/message.h>
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
  gloox::Message m(gloox::Message::Normal, gloox::JID(receiver), msg);
  assert( conn_.get()!=NULL );
  conn_.get()->send(m);
  // all done
  LOGMSG_DEBUG_S(log_)<<"message to user "<<receiver<<" sent";
}

void MessageIO::discardIncommingMessages(void)
{
  assert( conn_.get()!=NULL );
  LOGMSG_DEBUG(log_, "discarding all incomming messages");

  // TODO: comment this code, since it's non obvious at the first sight.
  /*
  while(g_main_context_iteration(NULL, FALSE))
  {
    LOGMSG_DEBUG(log_, "incomming message discarded");
  }
  */
  LOGMSG_DEBUG(log_, "all messages have been discarded");
}

} // namespace Jabber
} // namespace Trigger
