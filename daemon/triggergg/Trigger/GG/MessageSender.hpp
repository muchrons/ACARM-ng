/*
 * MessageSender.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_MESSAGESENDER_HPP_FILE
#define INCLUDE_TRIGGER_GG_MESSAGESENDER_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Trigger/GG/Connection.hpp"
#include "Trigger/GG/UserID.hpp"
#include "Trigger/GG/ExceptionSendingError.hpp"

// TODO: tests
// TODO: comments

namespace Trigger
{
namespace GG
{

class MessageSender: private boost::noncopyable
{
public:
  explicit MessageSender(Connection &conn);

  void send(UserID receiver, const std::string &msg);

private:
  Connection &conn_;
}; // class MessageSender

} // namespace GG
} // namespace Trigger

#endif
