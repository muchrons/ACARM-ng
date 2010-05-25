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


namespace
{
inline std::string addHeaders(const std::string &content,
                              const std::string &subject,
                              const std::string &from,
                              const std::string &to)
{
  const char *eol="\r\n";
  std::stringstream ss;
  ss<<"Subject: [acarmng] "<<subject<<eol;
  ss<<"From: "<<from<<eol;
  ss<<"To: "  <<to  <<eol;
  ss<<content;
  return ss.str();
} // addHeaders()
} // unnamed namespace


void MailSender::send(const std::string &subject, const std::string &content)
{
  const Config::Server        &srv =cfg_.getServerConfig();
  const Config::Authorization *auth=cfg_.getAuthorizationConfig();
  MailSmtp                     ms;

  // connect to server
  switch( srv.sec_.toInt() )
  {
    case Config::Server::Security::STARTTLS:
      connectionErrorHandler( ( mailsmtp_socket_connect( ms.get(), srv.server_.c_str(), srv.port_ ) ),
                                "mailsmtp_socket_connect" );
      break;

    case Config::Server::Security::SSL:
      connectionErrorHandler( ( mailsmtp_ssl_connect( ms.get(), srv.server_.c_str(), srv.port_ ) ),
                                "mailsmtp_ssl_connect" );
      break;

    default:
      assert(!"invalid/unknown security type - code must be updated first");
      throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, srv.server_.c_str(),
                                     "unknown security mode requested");
      break;
  } // switch(security_type)

  // proceed with protocol:
  errorHandler( mailesmtp_ehlo( ms.get() ), "mailesmtp_ehlo" ); // EHLO
  if(srv.sec_==Config::Server::Security::STARTTLS)              // STARTTLS?
    errorHandler( mailsmtp_socket_starttls( ms.get() ), "mailsmtp_socket_starttls");
  if(auth!=NULL)                                                // require authorization?
    errorHandler( mailsmtp_auth( ms.get(), auth->user_.c_str(), auth->pass_.c_str() ),
                  "mailsmtp_auth" );                            // AUTH
  errorHandler( mailesmtp_mail( ms.get(), srv.from_.c_str(), 0, NULL ),
                "mailesmtp_mail" );                             // FROM
  errorHandler( mailesmtp_rcpt( ms.get(), cfg_.getRecipientAddress().c_str(),
                                MAILSMTP_DSN_NOTIFY_FAILURE|MAILSMTP_DSN_NOTIFY_DELAY, NULL ),
                "mailesmtp_rcpt" );                             // TO
  errorHandler( mailsmtp_data( ms.get() ), "mailsmtp_data" );   // DATA
  // data-part headers and stuff...
  const std::string &whole=addHeaders(content, subject, srv.from_, cfg_.getRecipientAddress() );
  errorHandler( mailsmtp_data_message( ms.get(), whole.c_str(), whole.length() ),
                "mailsmtp_data_message" );                      // message body goes here

  errorHandler( mailsmtp_quit( ms.get() ), "mailsmtp_quit" );   // QUIT
}

void MailSender::connectionErrorHandler(int ret, const char *call) const
{
  if( !isError(ret) )
    return;
  // oops - we have a problem
  std::string err(call);
  err.append("(): ");
  err.append( mailsmtp_strerror(ret) );
  const Base::StrError se;
  err.append("; ");
  err.append( se.get() );
  throw ExceptionConnectionError( SYSTEM_SAVE_LOCATION,
                                  cfg_.getServerConfig().server_.c_str(),
                                  se.get().c_str() );
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
