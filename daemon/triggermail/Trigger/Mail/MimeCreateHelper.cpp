/*
 * mimeCreateHelpers.cpp
 *
 * NOTE: parts of this code are taken from libetpan exampes.
 *
 */
#include <cstring>
#include <cassert>

#include "System/AutoCptr.hpp"
#include "System/ScopedPtrCustom.hpp"
#include "Base/StrError.hpp"
#include "Trigger/Mail/MailSmtp.hpp"
#include "Trigger/Mail/MailSender.hpp"
#include "Trigger/Mail/MimeCreateHelper.hpp"

using namespace System;

namespace Trigger
{
namespace Mail
{

namespace
{

// helper typedef to make code shorter
typedef ScopedPtrCustom<mailmime, mailmime_free> MailMime;

// helper that returns malloc'ed string that is non-const
AutoCptr<char> getString(const std::string &in)
{
  AutoCptr<char> out( strdup( in.c_str() ) );
  if( out.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "strdup() failed");
  return out;
} // getString()

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


std::string MimeCreateHelper::createMimeMessage(void)
{
  // make local copies of these fields - they have to be passed as 'char*', since
  // lietpan is not const-correct, but ownership is not taken, so they have to be
  // released here anyhow.
  System::AutoCptr<char> from( getString(fromSrc_) );
  System::AutoCptr<char> to( getString(toSrc_) );
  System::AutoCptr<char> content( getString(contentSrc_) );
  // use these instead of normal 'const char*' since libetpan is not const-correct
  char                   charsetStr[] ="charset";
  char                   charsetType[]="utf-8";

  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> fields( buildFields( from.get(), to.get() ) );
  assert( fields.get()!=NULL );

  MailMime message( buildMessage( fields.release() ) );
  assert( message.get()!=NULL );

  MailMime textPart( buildBodyText( content.get(), charsetStr, charsetType ) );
  assert( textPart.get()!=NULL );

  if( mailmime_smart_add_part( message.get(), textPart.get() ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_smart_add_part() failed");
  textPart.release();

  // convert outptu to string and return to the user
  return convertToString( message.get() );
}


mailimf_fields *MimeCreateHelper::buildFields(char *fromPtr, char *toPtr)
{
  // from field: init
  ScopedPtrCustom<mailimf_mailbox_list, mailimf_mailbox_list_free> from( mailimf_mailbox_list_new_empty() );
  if( from.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_mailbox_list_new_empty() failed");
  // from field: add address
  assert( fromPtr!=NULL );
  if( mailimf_mailbox_list_add_parse( from.get(), fromPtr ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_mailbox_list_add_parse() failed");

  // to field: init
  ScopedPtrCustom<mailimf_address_list, mailimf_address_list_free> to( mailimf_address_list_new_empty() );
  if( to.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_address_list_new_empty() failed");
  // to field: add address
  assert( toPtr!=NULL );
  if( mailimf_address_list_add_parse( to.get(), toPtr ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_address_list_add_parse() failed");

  // make common structure allocation all parts
  System::AutoCptr<char> subject( getString(subjectSrc_) );
  assert( subject.get()!=NULL );
  mailimf_fields *newFields=mailimf_fields_new_with_data( from.get(),   // from,
                                                          NULL,         // sender,
                                                          NULL,         // reply-to,
                                                          to.get(),
                                                          NULL,         // cc,
                                                          NULL,         // bcc,
                                                          NULL,         // in-reply-to,
                                                          NULL,         // references,
                                                          subject.get() );
  if(newFields==NULL)
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailimf_fields_new_with_data() failed");
  // if allocation succedded, ensure elements are not deallocated
  subject.release();   // NOTE: this will be deallocated along with 'newFields' object.
  from.release();
  to.release();

  // return pointer to newly allocated elements
  return newFields;
} // buildFields()


// text is a string, build a mime part containing this string
mailmime *MimeCreateHelper::buildBodyText(char *contentPtr, char *charsetStr, char *charsetType)
{
  ScopedPtrCustom<mailmime_fields, mailmime_fields_free> mimeFields( mailmime_fields_new_encoding(MAILMIME_MECHANISM_8BIT) );
  if( mimeFields.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_fields_new_encoding() failed");

  ScopedPtrCustom<mailmime_content, mailmime_content_free> content( mailmime_content_new_with_str("text/plain") );
  if( content.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_content_new_with_str() failed");

  assert( charsetStr !=NULL );
  assert( charsetType!=NULL );
  ScopedPtrCustom<mailmime_parameter, mailmime_parameter_free> param( mailmime_param_new_with_data(charsetStr, charsetType) );
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

  assert( contentPtr!=NULL );
  if( mailmime_set_body_text( mimeSub.get(), contentPtr, strlen(contentPtr) ) != MAILIMF_NO_ERROR )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_set_body_text() failed");

  return mimeSub.release();
}


mailmime *MimeCreateHelper::buildMessage(mailimf_fields *fields)
{
  ScopedPtrCustom<mailimf_fields, mailimf_fields_free> fieldsPtr(fields);
  MailMime                                             mime( mailmime_new_message_data(NULL) );
  if( mime.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mailmime_new_message_data() failed");

  mailmime_set_imf_fields( mime.get(), fieldsPtr.release() );   // this call always succeeds
  return mime.release();
}


std::string MimeCreateHelper::convertToString(mailmime *msg)
{
  assert(msg!=NULL);
  ScopedPtrCustom<MMAPString, mmap_string_free> mmstr( mmap_string_new("") );
  if( mmstr.get()==NULL )
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, "mmap_string_new() failed");

  int col=0;
  mailmime_write_mem( mmstr.get(), &col, msg );

  return mmstr->str;
}

} // namespace Mail
} // namespace Trigger
