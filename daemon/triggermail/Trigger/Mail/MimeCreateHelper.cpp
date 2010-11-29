/*
 * mimeCreateHelpers.cpp
 *
 */
#include "Trigger/Mail/MimeCreateHelper.hpp"

namespace Trigger
{
namespace Mail
{

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
  // TODO: write this code
  return "TODO";
}

} // namespace Mail
} // namespace Trigger
