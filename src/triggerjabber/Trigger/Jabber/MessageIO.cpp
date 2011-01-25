/*
 * MessageIO.cpp
 *
 */
#include <sys/select.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Logger/Logger.hpp"
#include "Trigger/Jabber/Gloox.hpp"
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
  gloox::Message m(gloox::Message::Chat, gloox::JID(receiver), msg);
  assert( conn_.get()!=NULL );
  discardIncommingMessages();
  // TODO: use arrow operator instaed of explicitly calling get().
  conn_.get()->send(m);
  discardIncommingMessages();
  // all done
  LOGMSG_DEBUG_S(log_)<<"message to user "<<receiver<<" sent";
}

void MessageIO::discardIncommingMessages(void)
{
  assert( conn_.get()!=NULL );
  LOGMSG_DEBUG(log_, "discarding all incomming messages");
  // TODO: use arrow operator instaed of explicitly calling get().
  conn_.get()->recv(1000);
  LOGMSG_DEBUG(log_, "all messages have been discarded");
}

} // namespace Jabber
} // namespace Trigger
