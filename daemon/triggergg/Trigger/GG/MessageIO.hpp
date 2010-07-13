/*
 * MessageIO.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_GG_MESSAGEIO_HPP_FILE
#define INCLUDE_TRIGGER_GG_MESSAGEIO_HPP_FILE

#include <string>
#include <boost/noncopyable.hpp>

#include "Logger/Node.hpp"
#include "Trigger/GG/Connection.hpp"
#include "Trigger/GG/UserID.hpp"
#include "Trigger/GG/ExceptionSendingError.hpp"
#include "Trigger/GG/ExceptionReceivingError.hpp"

namespace Trigger
{
namespace GG
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
  void send(UserID receiver, const std::string &msg);

  /** \brief receive all incomming messages and discard them.
   *  \note call is non-blocking.
   */
  void discardIncommingMessages(void);

private:
  Logger::Node  log_;
  Connection   &conn_;
}; // class MessageIO

} // namespace GG
} // namespace Trigger

#endif
