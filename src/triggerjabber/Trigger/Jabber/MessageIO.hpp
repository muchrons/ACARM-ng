/*
 * MessageIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_JABBER_MESSAGEIO_HPP_FILE
#define INCLUDE_TRIGGER_JABBER_MESSAGEIO_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Trigger/Jabber/Connection.hpp"
#include "Trigger/Jabber/ExceptionCreatingError.hpp"

namespace Trigger
{
namespace Jabber
{

/** \brief wrapper for message I/O mechanism.
 */
class MessageIO: private boost::noncopyable
{
public:
  /** \brief create sender for given connection.
   *  \param conn connection to be used for sending messages.
   *  \note conn must be valid reference as long as MessageIO object lives.
   */
  explicit MessageIO(Connection &conn);

  /** \brief send message to a givne user.
   *  \param receiver user that is supposed to recieve given message.
   *  \param msg      message to be send.
   */
  void send(const std::string &receiver, const std::string &msg);

  /** \brief receive all incomming messages and discard them.
   *  \note call is non-blocking.
   */
  void discardIncommingMessages(void);

private:

  Logger::Node  log_;
  Connection   &conn_;
}; // class MessageIO

} // namespace Jabber
} // namespace Trigger

#endif
