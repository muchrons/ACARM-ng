/*
 * MailSender.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_MAILSENDER_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_MAILSENDER_HPP_FILE

#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/ExceptionConnectionError.hpp"
#include "Trigger/Mail/ExceptionSendingError.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief wrapper for sending messages to destination.
 */
class MailSender
{
public:
  /** \brief create sender with given configuration.
   *  \param cfg configuration to be used.
   */
  explicit MailSender(const Config &cfg);

  /** \brief send message to configured recipient.
   *  \param subject message's subject (note that '[acarmng]' prefix will be
   *         added in from of it automaticaly).
   *  \param content message's content.
   */
  void send(const std::string &subject, const std::string &content);

private:
  void connectionErrorHandler(int ret, const char *call) const;
  void errorHandler(int ret, const char *call) const;
  bool isError(int ret) const;

  const Config cfg_;
}; // class MailSmtp

} // namespace Mail
} // namespace Trigger

#endif
