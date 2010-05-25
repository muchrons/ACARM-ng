/*
 * MailSmtp.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_MAILSMTP_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_MAILSMTP_HPP_FILE

#include <boost/noncopyable.hpp>
#include <libetpan/libetpan.h>
#include <cassert>

#include "Trigger/Mail/LoggerWrapper.hpp"
#include "Trigger/Mail/Exception.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief wrapper for 'mailsmtp' C-type from libetpan.
 */
class MailSmtp: private boost::noncopyable
{
public:
  /** \brief create handler.
   */
  MailSmtp(void);
  /** \brief deallocate handler.
   */
  ~MailSmtp(void);

  /** \brief gets pointer to internal data - non-const version.
   *  \return pointer to mailsmtp strucutre.
   *  \note pointer is guaranteed to always be non-NULL.
   */
  mailsmtp *get(void)
  {
    assert(ms_!=NULL);
    return ms_;
  }
  /** \brief gets pointer to internal data - const version.
   *  \return pointer to mailsmtp strucutre.
   *  \note pointer is guaranteed to always be non-NULL.
   */
  const mailsmtp *get(void) const
  {
    assert(ms_!=NULL);
    return ms_;
  }

private:
  const LoggerWrapper  lw_;
  mailsmtp            *ms_;
}; // class MailSmtp

} // namespace Mail
} // namespace Trigger

#endif
