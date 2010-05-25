/*
 * MailSmtp.cpp
 *
 */
#include <cassert>

#include "Base/StrError.hpp"
#include "Trigger/Mail/MailSmtp.hpp"

namespace Trigger
{
namespace Mail
{

MailSmtp::MailSmtp(void):
  ms_( mailsmtp_new(0, NULL) )
{
  if(ms_==NULL)
  {
    const Base::StrError se;
    throw Exception(SYSTEM_SAVE_LOCATION, se.get() );
  }
}

MailSmtp::~MailSmtp(void)
{
  assert(ms_!=NULL);
  mailsmtp_free(ms_);
}

} // namespace Mail
} // namespace Trigger
