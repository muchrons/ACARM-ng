/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE

#include <string>
#include <inttypes.h>

#include "Trigger/Simple/ThresholdConfig.hpp"

// TODO: comments

namespace Trigger
{
namespace Mail
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  struct Required
  {
    Required(const std::string &from,
             const std::string &to,
             const std::string &server,
             const uint16_t     port,
             bool               useTLS):
      from_(from),
      to_(to),
      server_(server),
      port_(port),
      useTLS_(useTLS)
    {
    }

    const std::string from_;
    const std::string to_;
    const std::string server_;
    const uint16_t    port_;
    bool              useTLS_;
  }; // struct Required

  struct Authorization
  {
    Authorization(const std::string user,
                  const std::string pass):
      user_(user),
      pass_(pass)
    {
    }

    const std::string user_;
    const std::string pass_;
  }; // struct Authorization

  /** \brief create configration description, without authorization.
   *  \param th  threshold configuration - informs when run trigger.
   *  \param req set of required parameters.
   */
  Config(const Simple::ThresholdConfig &th,
         const Required                &req):
    th_(th),
    req_(req),
    useAuth_(false),
    auth_("", "")
  {
  }
  /** \brief create configration description, with authorization request.
   *  \param th   threshold configuration - informs when run trigger.
   *  \param req  set of required parameters.
   *  \param auth parameters required for authorization.
   */
  Config(const Simple::ThresholdConfig &th,
         const Required                &req,
         const Authorization           &auth):
    th_(th),
    req_(req),
    useAuth_(true),
    auth_(auth)
  {
  }

  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }
  /** \brief get authorization configuration.
   *  \return authorization parameters. NULL, if authorization is not requested.
   */
  const Authorization *getAuthorizationConfig(void) const
  {
    if(useAuth_)
      return &auth_;
    return NULL;
  }
  /** \brief get required configuration part.
   *  \return required parameters from the configuration.
   */
  const Required &getRequiredConfig(void) const
  {
    return req_;
  }

private:
  Simple::ThresholdConfig th_;
  Required                req_;
  bool                    useAuth_;
  Authorization           auth_;
}; // class Config

} // namespace Mail
} // namespace Trigger

#endif
