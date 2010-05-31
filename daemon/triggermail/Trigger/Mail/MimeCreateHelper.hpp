/*
 * MimeCreateHelpers.hpp
 *
 */
#include <string>
#include <boost/noncopyable.hpp>
#include <libetpan/libetpan.h>

namespace Trigger
{
namespace Mail
{

class MimeCreateHelper: private boost::noncopyable
{
public:
  /** \brief prepare data for creation of mime message.
   *  \param from    sender's e-mail.
   *  \param to      receiver's e-mail.
   *  \param subject message's subject.
   *  \param content message's content (aka: body part).
   */
  MimeCreateHelper(const std::string &from,
                   const std::string &to,
                   const std::string &subject,
                   const std::string &content);

  /** \brief construct message and return it as a string.
   *  \return mime-formatted message.
   */
  std::string createMimeMessage(void);

private:
  // NOTE: call returns pointer along with ownership.
  mailimf_fields *buildFields(char *fromPtr, char *toPtr);

  // text is a string, build a mime part containing this string
  // NOTE: call returns pointer along with ownership.
  mailmime *buildBodyText(char *contentPtr, char *charsetStr, char *charsetType);

  // build an empty message
  // NOTE: call returns pointer along with ownership.
  mailmime *buildMessage(mailimf_fields *fields);

  const std::string fromSrc_;
  const std::string toSrc_;
  const std::string subjectSrc_;
  const std::string contentSrc_;
}; // class MimeCreateHelper

} // namespace Mail
} // namespace Trigger
