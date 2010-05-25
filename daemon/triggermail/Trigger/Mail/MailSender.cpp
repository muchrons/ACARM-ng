/*
 * MailSender.cpp
 *
 */
#include "Base/StrError.hpp"
#include "Trigger/Mail/MailSmtp.hpp"
#include "Trigger/Mail/MailSender.hpp"

namespace Trigger
{
namespace Mail
{

MailSender::MailSender(const Config &cfg):
  cfg_(cfg)
{
}

void MailSender::send(const std::string &subject, const std::string &content)
{
  const Config::Server        &srv =cfg_.getServerConfig();
  const Config::Authorization *auth=cfg_.getAuthorizationConfig();
  MailSmtp                     ms;

  // TODO: add switching between ssl/raw connection

  // connect to server
  if( isError( mailsmtp_socket_connect( ms.get(), srv.server_.c_str(), srv.port_ ) ) )
  {
    const Base::StrError se;
    throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, srv.server_.c_str(), se.get().c_str() );
  }
  // proceed with protocol:
  errorHandler( mailesmtp_ehlo( ms.get() ), "mailesmtp_ehlo" ); // EHLO
  if(srv.sec_==Config::Server::Security::STARTTLS)              // STARTTLS?
    errorHandler( mailsmtp_socket_starttls( ms.get() ), "mailsmtp_socket_starttls");
  if(auth!=NULL)                                                // require authorization?
    errorHandler( mailsmtp_auth( ms.get(), auth->user_.c_str(), auth->pass_.c_str() ), "mailsmtp_auth" );
  errorHandler( mailesmtp_mail( ms.get(), srv.from_.c_str(), 1, "ACARM-ng" ), "mailesmtp_mail" );
  errorHandler( mailesmtp_rcpt( ms.get(), cfg_.getRecipientAddress().c_str(),
                                MAILSMTP_DSN_NOTIFY_FAILURE|MAILSMTP_DSN_NOTIFY_DELAY, NULL ),
                "mailesmtp_rcpt" );
  errorHandler( mailsmtp_data( ms.get() ), "mailsmtp_data" );
  errorHandler( mailsmtp_data_message( ms.get(), content.c_str(), content.length() ),
                "mailsmtp_data_message" );
  // TODO: setup subject too
}

void MailSender::errorHandler(const int ret, const char *call) const
{
  if( !isError(ret) )
    return;
  // oops - we have a problem
  std::string err(call);
  err.append("(): ");
  err.append( mailsmtp_strerror(ret) );
  throw ExceptionSendingError( SYSTEM_SAVE_LOCATION,
                               cfg_.getServerConfig().from_.c_str(),
                               cfg_.getRecipientAddress().c_str(),
                               err.c_str() );
}

bool MailSender::isError(const int ret) const
{
  return ret!=MAILSMTP_NO_ERROR;
}

} // namespace Mail
} // namespace Trigger
