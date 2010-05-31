/*
 * mimeCreateHelpers.cpp
 *
 * NOTE: parts of this code are taken from libetpan exampes.
 *
 */
#include <cstring>
#include <cassert>

#include "System/ScopedPtrCustom.hpp"
#include "System/AutoCptr.hpp"
#include "Base/StrError.hpp"
#include "Trigger/Mail/MailSmtp.hpp"
#include "Trigger/Mail/MailSender.hpp"
#include "Trigger/Mail/MimeCreateHelper.hpp"

// TODO: fix mem-leaks in this file

using namespace System;

namespace Trigger
{
namespace Mail
{

namespace
{

typedef ScopedPtrCustom<mailmime, mailmime_free> MailMime;


// helper that returns malloc'ed string that is non-const
AutoCptr<char> getString(const std::string &in)
{
  AutoCptr<char> out( strdup( in.c_str() ) );
  if( out.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "strdup() failed");
  return out;
} // getString()

template<typename T>
void setCString(T &out, const char *in)
{
  assert( sizeof(out)>strlen(in) && "given stirng is too long to fit in buffer" );
  strcpy(out, in);
} // setCString()

} // unnamed namespace


MimeCreateHelper::MimeCreateHelper(const std::string &from,
                                   const std::string &to,
                                   const std::string &subject,
                                   const std::string &content):
  fromSrc_(from),
  toSrc_(to),
  subjectSrc_(subject),
  contentSrc_(content)
{
}

MimeCreateHelper::~MimeCreateHelper(void)
{
  //from_.release();
  //to_.release();
  //subject_.release();
  //content_.release();
}


std::string MimeCreateHelper::createMimeMessage(void)
{
  initData();

  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> fields( buildFields() );
  assert( fields.get()!=NULL );

  MailMime message( buildMessage( fields.release() ) );
  assert( message.get()!=NULL );

  MailMime textPart( buildBodyText() );
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
}


void MimeCreateHelper::initData(void)
{
  // NOTE: these fields cannot be just const char *, since libetpan is not const-correct
  setCString(charsetStr_,  "charset");
  setCString(charsetType_, "utf-8");

  // sanity check for allocated memory parts

  from_=getString(fromSrc_);
  if( from_.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "unable to copy 'from' string");

  to_=getString(toSrc_);
  if( to_.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "unable to copy 'to' string");

  subject_=getString(subjectSrc_);
  if( subject_.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "unable to copy 'subject' string");

  content_=getString(contentSrc_);
  if( content_.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "unable to copy 'content' string");
}


mailimf_fields *MimeCreateHelper::buildFields(void)
{
  // from field: init
  ScopedPtrCustom<mailimf_mailbox_list, mailimf_mailbox_list_free> from( mailimf_mailbox_list_new_empty() );
  if( from.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_mailbox_list_new_empty() failed");
  // from field: add address
  assert( from_.get()!=NULL );
  if( mailimf_mailbox_list_add_parse( from.get(), from_.get() ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_mailbox_list_add_parse() failed");
  from_.release();      // NOTE: this will be deallocated along with 'from' object.

  // to field: init
  ScopedPtrCustom<mailimf_address_list, mailimf_address_list_free> to( mailimf_address_list_new_empty() );
  if( to.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_address_list_new_empty() failed");
  // to field: add address
  assert( to_.get()!=NULL );
  if( mailimf_address_list_add_parse( to.get(), to_.get() ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_address_list_add_parse() failed");
  to_.release();        // NOTE: this will be deallocated along with 'to' object.

  // make common structure allocation all parts
  assert( subject_.get()!=NULL );
  mailimf_fields *newFields=mailimf_fields_new_with_data( from.get(),   // from,
                                                          NULL,         // sender,
                                                          NULL,         // reply-to,
                                                          to.get(),
                                                          NULL,         // cc,
                                                          NULL,         // bcc,
                                                          NULL,         // in-reply-to,
                                                          NULL,         // references,
                                                          subject_.get() );
  if(newFields==NULL)
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_fields_new_with_data() failed");
  // if allocation succedded, ensure elements are not deallocated
  subject_.release();   // NOTE: this will be deallocated along with 'newFields' object.
  from.release();
  to.release();

  // return pointer to newly allocated elements
  return newFields;
} // buildFields()


// text is a string, build a mime part containing this string
mailmime *MimeCreateHelper::buildBodyText(void)
{
  ScopedPtrCustom<mailmime_fields, mailmime_fields_free> mimeFields( mailmime_fields_new_encoding(MAILMIME_MECHANISM_8BIT) );
  if( mimeFields.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_fields_new_encoding() failed");

  ScopedPtrCustom<mailmime_content, mailmime_content_free> content( mailmime_content_new_with_str("text/plain") );
  if( content.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_content_new_with_str() failed");

  ScopedPtrCustom<mailmime_parameter, mailmime_parameter_free> param( mailmime_param_new_with_data(charsetStr_, charsetType_) );
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

  if( mailmime_set_body_text( mimeSub.get(), content_.get(), strlen( content_.get() ) ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_set_body_text() failed");
  content_.release();       // NOTE: this will be deallocated along with 'mimeSub' object.

  return mimeSub.release();
}


mailmime *MimeCreateHelper::buildMessage(mailimf_fields *fields)
{
  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> fieldsPtr(fields);
  MailMime                                             mime( mailmime_new_message_data(NULL) );
  if( mime.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_new_message_data() failed");

  mailmime_set_imf_fields( mime.get(), fieldsPtr.release() );
  // TODO: check if error checking is really not needed here.
  return mime.release();
}

} // namespace Mail
} // namespace Trigger
