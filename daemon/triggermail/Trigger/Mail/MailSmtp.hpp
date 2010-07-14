/*
 * MailSmtp.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_MAILSMTP_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_MAILSMTP_HPP_FILE

#include <libetpan/libetpan.h>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "Trigger/Mail/LoggerWrapper.hpp"
#include "Trigger/Mail/Exception.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief wrapper for 'mailsmtp' C-type from libetpan.
 */
class MailSmtp: public System::ScopedPtrCustom<mailsmtp, mailsmtp_free>
{
public:
  /** \brief create handle.
   */
  MailSmtp(void);

private:
  const LoggerWrapper  lw_;
}; // class MailSmtp

} // namespace Mail
} // namespace Trigger

#endif
