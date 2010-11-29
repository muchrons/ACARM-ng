/*
 * MailSender.cpp
 *
 */
#include <cstring>

#include "Trigger/Mail/MailSender.hpp"
#include "Trigger/Mail/MimeCreateHelper.hpp"    // TODO: needed?

using namespace System;

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
  try
  {
  const Config::Server        &srv =cfg_.getServerConfig();
  const Config::Authorization *auth=cfg_.getAuthorizationConfig();

  // connect to server
  switch( srv.sec_.toInt() )
  {
    case Config::Server::Security::STARTTLS:
      // TODO
      break;

    case Config::Server::Security::SSL:
      // TODO
      break;

    default:
      assert(!"invalid/unknown security type - code must be updated first");
      throw ExceptionConnectionError(SYSTEM_SAVE_LOCATION, srv.server_.c_str(),
                                     "unknown security mode requested");
      break;
  } // switch(security_type)

  // TODO...
  }
  catch(const vmime::exception &ex)
  {
    // TODO
    // translate vmime-specific exception to project-wide exception
    throw ;//ExceptionConnectionError
  }
}

} // namespace Mail
} // namespace Trigger
