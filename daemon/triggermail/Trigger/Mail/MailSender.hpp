/*
 * MailSender.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_MAILSENDER_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_MAILSENDER_HPP_FILE

#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/LoggerWrapper.hpp"
#include "Trigger/Mail/ExceptionConnectionError.hpp"
#include "Trigger/Mail/ExceptionSendingError.hpp"

// TODO: comments

namespace Trigger
{
namespace Mail
{

class MailSender
{
public:
  explicit MailSender(const Config &cfg);

  void send(const std::string &subject, const std::string &content);

private:
  void errorHandler(int ret, const char *call) const;
  bool isError(int ret) const;

  const Config        cfg_;
  const LoggerWrapper lw_;
}; // class MailSmtp

} // namespace Mail
} // namespace Trigger

#endif
