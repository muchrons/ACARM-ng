/*
 * MailSmtp.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_MAILSMTP_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_MAILSMTP_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libetpan/libetpan.h>
#include <cassert>

#include "Trigger/Mail/Exception.hpp"

// TODO: comments

namespace Trigger
{
namespace Mail
{

class MailSmtp: private boost::noncopyable
{
public:
  MailSmtp(void);
  ~MailSmtp(void);

  mailsmtp *get(void)
  {
    assert(ms_!=NULL);
    return ms_;
  }
  const mailsmtp *get(void) const
  {
    assert(ms_!=NULL);
    return ms_;
  }

private:
  mailsmtp *ms_;
}; // class MailSmtp

} // namespace Mail
} // namespace Trigger

#endif
