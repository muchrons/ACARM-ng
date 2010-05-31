/*
 * mimeCreateHelpers.hpp
 *
 */
#include <string>
#include <libetpan/libetpan.h>

namespace Trigger
{
namespace Mail
{

// (NOTE: call returns pointer along with ownership).
mailimf_fields *buildFields(const std::string &fromParam,
                            const std::string &toParam,
                            const std::string &subjectParam);

// text is a string, build a mime part containing this string
// (NOTE: call returns pointer along with ownership).
mailmime *buildBodyText(const std::string &text);

// build an empty message (NOTE: call returns pointer along with ownership).
mailmime *buildMessage(mailimf_fields *fields);

// create whole mime-message and return as string.
std::string createMimeMessage(const std::string &from,
                              const std::string &to,
                              const std::string &subject,
                              const std::string &content);

} // namespace Mail
} // namespace Trigger
