/*
 * MessageSender.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_MESSAGESENDER_HPP_FILE
#define INCLUDE_TRIGGER_GG_MESSAGESENDER_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Trigger/GG/Connection.hpp"
#include "Trigger/GG/UserID.hpp"
#include "Trigger/GG/ExceptionSendingError.hpp"

namespace Trigger
{
namespace GG
{

/** \brief wrapper for message sending mechanism.
 */
class MessageSender: private boost::noncopyable
{
public:
  /** \brief create sender for given connection.
   *  \param conn connection to be used for sending messages.
   *  \note conn must be valid reference as long as MessageSender object lives.
   */
  explicit MessageSender(Connection &conn);

  /** \brief send message to a givne user.
   *  \param receiver user that is supposed to recieve given message.
   *  \param msg      message to be send.
   */
  void send(UserID receiver, const std::string &msg);

private:
  void discardIncommingMessages(Logger::Node log);

  Connection &conn_;
}; // class MessageSender

} // namespace GG
} // namespace Trigger

#endif
