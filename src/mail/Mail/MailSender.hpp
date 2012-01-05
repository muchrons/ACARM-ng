/*
 * MailSender.hpp
 *
 */
#ifndef INCLUDE_MAIL_MAILSENDER_HPP_FILE
#define INCLUDE_MAIL_MAILSENDER_HPP_FILE

#include "Logger/Node.hpp"
#include "Mail/Config.hpp"
#include "Mail/VmimeHandleInit.hpp"
#include "Mail/ExceptionConnectionError.hpp"
#include "Mail/ExceptionInvalidCertificate.hpp"
#include "Mail/ExceptionSendingError.hpp"
#include "Mail/ExceptionUnableToCreateMessage.hpp"

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
  Logger::Node    log_;
  VmimeHandleInit vhi_;
  const Config    cfg_;
}; // class MailSmtp

} // namespace Mail

#endif
