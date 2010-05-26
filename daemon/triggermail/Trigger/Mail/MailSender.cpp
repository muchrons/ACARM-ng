/*
 * MailSender.cpp
 *
 * NOTE: parts of this code are taken from libetpan exampes.
 *
 */
#include <cstring>

#include "System/ScopedPtrCustom.hpp"
#include "System/AutoCptr.hpp"
#include "Base/StrError.hpp"
#include "Trigger/Mail/MailSmtp.hpp"
#include "Trigger/Mail/MailSender.hpp"

// TODO: fix mem-leaks in this file

using namespace System;

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

typedef ScopedPtrCustom<mailmime, mailmime_free> MailMime;


// NOTE: this cannot be const char *, since libetpan is not const-correct
char charset[]="utf-8";


// helper that returns malloc'ed string that is non-const
AutoCptr<char> getString(const std::string &in)
{
  AutoCptr<char> out( strdup( in.c_str() ) );
  if( out.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "strdup() failed");
  return out;
} // getString()


mailimf_fields *buildFields(const std::string &fromParam,
                            const std::string &toParam,
                            const std::string &subjectParam)
{
  // fields that are required as 'char*', instead of 'const char*'
  AutoCptr<char> subjectPtr=getString( "[acarmng] " + subjectParam);
  AutoCptr<char> fromPtr   =getString( "ACARM-ng <" + fromParam + ">" );
  AutoCptr<char> toPtr     =getString(toParam);

  // from field: init
  ScopedPtrCustom<mailimf_mailbox_list, mailimf_mailbox_list_free> from( mailimf_mailbox_list_new_empty() );
  if( from.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_mailbox_list_new_empty() failed");
  // from field: add address
  if( mailimf_mailbox_list_add_parse( from.get(), fromPtr.get() ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_mailbox_list_add_parse() failed");
  fromPtr.release();

  // to field: init
  ScopedPtrCustom<mailimf_address_list, mailimf_address_list_free> to( mailimf_address_list_new_empty() );
  if( to.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_address_list_new_empty() failed");
  // to field: add address
  if( mailimf_address_list_add_parse( to.get(), toPtr.get() ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_address_list_add_parse() failed");
  toPtr.release();

  // make common structure allocation all parts
  mailimf_fields *newFields=mailimf_fields_new_with_data( from.get(),   // from,
                                                          NULL,         // sender,
                                                          NULL,         // reply-to,
                                                          to.get(),
                                                          NULL,         // cc,
                                                          NULL,         // bcc,
                                                          NULL,         // in-reply-to,
                                                          NULL,         // references,
                                                          subjectPtr.get() );
  if(newFields==NULL)
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_fields_new_with_data() failed");
  // if allocation succedded, ensure elements are not deallocated
  subjectPtr.release();
  from.release();
  to.release();

  // return pointer to newly allocated elements
  return newFields;
} // buildFields()


// text is a string, build a mime part containing this string
struct mailmime *buildBodyText(const std::string &text)
{
  AutoCptr<char> textPtr=getString(text);

  ScopedPtrCustom<mailmime_fields, mailmime_fields_free> mimeFields( mailmime_fields_new_encoding(MAILMIME_MECHANISM_8BIT) );
  if( mimeFields.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_fields_new_encoding() failed");

  ScopedPtrCustom<mailmime_content, mailmime_content_free> content( mailmime_content_new_with_str("text/plain") );
  if( content.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_content_new_with_str() failed");

  AutoCptr<char> charsetPtr=getString("charset");
  ScopedPtrCustom<mailmime_parameter, mailmime_parameter_free> param( mailmime_param_new_with_data(charsetPtr.get(), charset) );
  if( param.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_param_new_with_data() failed");

  if( clist_append( content->ct_parameters, param.get() ) < 0 )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "clist_append() failed");
  param.release();

  ScopedPtrCustom<mailmime, mailmime_free> mimeSub( mailmime_new_empty( content.get(), mimeFields.get() ) );
  if( mimeSub.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_new_empty() failed");
  content.release();
  mimeFields.release();

  if( mailmime_set_body_text( mimeSub.get(), textPtr.get(), strlen( textPtr.get() ) ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_set_body_text() failed");
  textPtr.release();

  return mimeSub.release();
} // buildBodyText()


// build an empty message
mailmime *buildMessage(mailimf_fields *fields)
{
  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> fieldsPtr(fields);
  MailMime                                             mime( mailmime_new_message_data(NULL) );
  if( mime.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_new_message_data() failed");

  mailmime_set_imf_fields( mime.get(), fieldsPtr.release() );
  return mime.release();
} // buildMessage()


std::string createMimeMessage(const std::string &from,
                              const std::string &to,
                              const std::string &subject,
                              const std::string &content)
{
  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> fields( buildFields(from, to, subject) );
  assert( fields.get()!=NULL );

  MailMime message( buildMessage( fields.release() ) );
  assert( message.get()!=NULL );

  MailMime textPart( buildBodyText(content) );
  assert( textPart.get()!=NULL );

  if( mailmime_smart_add_part( message.get(), textPart.get() ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_smart_add_part() failed");
  textPart.release();

  // TODO: this is temporary code
  int col=0;
  mailmime_write( stdout, &col, message.get() );

  // TODO: do something to return real message instead of this hardcoded stuff...
  return "From: <acarmng.test.account1@gmail.com>\r\n"
         "To: acarmng.test.account2@gmail.com\r\n"
         "Subject: kszy\r\n"
         "ala ma kota\r\n"
         ;
} // createMimeMessage()

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
  const std::string &whole=createMimeMessage(srv.from_, cfg_.getRecipientAddress(), subject, content);
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
