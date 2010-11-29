/*
 * MimeCreateHelpers.hpp
 *
 */
#include <string>
#include <boost/noncopyable.hpp>
#include <vmime/vmime.hpp>

#include "Trigger/Mail/Config.hpp"
#include "Trigger/Mail/VmimeHandleInit.hpp"
#include "Trigger/Mail/ExceptionUnableToCreateMessage.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief class wrapping aroung all libetpan-specific elements creating mime message.
 */
class MimeCreateHelper: private boost::noncopyable
{
public:
  typedef vmime::ref<vmime::message> MessagePtr;

  /** \brief prepare data for creation of mime message.
   *  \param from    sender's e-mail.
   *  \param to      receiver's e-mail.
   *  \param subject message's subject.
   *  \param content message's content (aka: body part).
   */
  MimeCreateHelper(const std::string        &from,
                   const Config::Recipients &to,
                   const std::string        &subject,
                   const std::string        &content);

  /** \brief construct message and return it as a string.
   *  \return mime-formatted message.
   */
  MessagePtr createMimeMessage(void);

private:
  VmimeHandleInit          vhi_;
  const std::string        from_;
  const Config::Recipients to_;
  const std::string        subject_;
  const std::string        content_;
}; // class MimeCreateHelper

} // namespace Mail
} // namespace Trigger
