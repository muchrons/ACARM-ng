/*
 * mimeCreateHelpers.cpp
 *
 */
#include <cassert>

#include "Mail/MimeCreateHelper.hpp"

namespace Mail
{

namespace
{
vmime::ref<vmime::text> strUTF8(const std::string &str)
{
  vmime::ref<vmime::text>  utf=vmime::text::newFromString(str, "utf-8");
  return utf;
}

} // unnamed namespace


MimeCreateHelper::MimeCreateHelper(const std::string        &from,
                                   const Config::Recipients &to,
                                   const std::string        &subject,
                                   const std::string        &content):
  from_(from),
  to_(to),
  subject_(subject),
  content_(content)
{
}


MimeCreateHelper::MessagePtr MimeCreateHelper::createMimeMessage(void)
{
  try
  {
    // prepare message builder
    vmime::messageBuilder mb;

    // fill out content
    mb.setExpeditor( vmime::mailbox(from_) );                                         // set sender
    for(Config::Recipients::const_iterator it=to_.begin(); it!=to_.end(); ++it)       // set recipients
      mb.getRecipients().appendAddress( vmime::create<vmime::mailbox>(*it) );
    mb.setSubject( *strUTF8(subject_) );                                              // set subject
    mb.getTextPart()->setCharset(vmime::charsets::UTF_8);                             // set UTF-8 as charset
    mb.getTextPart()->setText( vmime::create<vmime::stringContentHandler>(content_) );// set message part

    // get final message
    return mb.construct();
  }
  catch(const vmime::exception &ex)
  {
    // translate vmime-specific exception to project-specific one
    throw ExceptionUnableToCreateMessage(SYSTEM_SAVE_LOCATION, ex.what() );
  }
}

} // namespace Mail
