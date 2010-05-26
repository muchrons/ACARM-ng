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
  System::ScopedPtrCustom<mailsmtp, mailsmtp_free>( mailsmtp_new(0, NULL) )
{
  if( get()==NULL )
  {
    const Base::StrError se;
    throw Exception(SYSTEM_SAVE_LOCATION, se.get() );
  }
}

} // namespace Mail
} // namespace Trigger
