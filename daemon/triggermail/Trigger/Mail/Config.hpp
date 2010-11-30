/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE

#include <string>
#include <inttypes.h>

#include "System/Enum.hpp"
#include "Base/NonEmptyVector.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace Mail
{
namespace detail
{
/** \brief implemenation helper.
 */
struct SecurityEnum
{
  /** \brief enum for security types declaration. */
  typedef enum
  {
    //NONE, // NOTE: non-secure connection is NOT allowed for security reasons
    TLS,
    SSL
  } Type;
}; // struct Security

/** \brief implemenation helper.
 */
struct ProtocolEnum
{
  /** \brief enum for security types declaration. */
  typedef enum
  {
    SMTP
  } Type;
}; // struct Security
} // namespace detail


/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief list of message recipients. */
  typedef Base::NonEmptyVector<std::string> Recipients;

  /** \brief server's configuration.
   */
  struct Server
  {
    /** \brief security type enum - user-level class. */
    typedef System::Enum<detail::SecurityEnum> Security;
    /** \brief protocol type enum - user-level class. */
    typedef System::Enum<detail::ProtocolEnum> Protocol;

    /** \brief create configuration from given paramters.
     *  \param server server address.
     *  \param port   port to connect to.
     *  \param proto  protocol type to use.
     *  \param sec    security type.
     */
    Server(const std::string &server,
           const uint16_t     port,
           Protocol           proto,
           Security           sec):
      server_(server),
      port_(port),
      proto_(proto),
      sec_(sec)
    {
    }

    const std::string server_;      ///< server's addres.
    const uint16_t    port_;        ///< port server's listening on.
    const Protocol    proto_;       ///< protocol type to be used.
    const Security    sec_;         ///< security type to be used.
  }; // struct Server

  /** \brief authorization settings.
   */
  struct Authorization
  {
    /** \brief create authroization configuration.
     *  \param user user's name.
     *  \param pass user's password.
     */
    Authorization(const std::string user,
                  const std::string pass):
      user_(user),
      pass_(pass)
    {
    }

    const std::string user_;    ///< user name.
    const std::string pass_;    ///< password for given user name.
  }; // struct Authorization

  /** \brief create configration description, without authorization.
   *  \param th   threshold configuration - informs when run trigger.
   *  \param from sender's e-mail address.
   *  \param to   recipient address.
   *  \param srv  server to connect to.
   */
  Config(const Simple::ThresholdConfig &th,
         const std::string             &from,
         const Recipients              &to,
         const Server                  &srv):
    th_(th),
    from_(from),
    to_(to),
    srv_(srv),
    useAuth_(false),
    auth_("", "")
  {
  }
  /** \brief create configration description, with authorization request.
   *  \param th   threshold configuration - informs when run trigger.
   *  \param from sender's e-mail address.
   *  \param to   recipient address.
   *  \param srv  server to connect to.
   *  \param auth parameters required for authorization.
   */
  Config(const Simple::ThresholdConfig &th,
         const std::string             &from,
         const Recipients              &to,
         const Server                  &srv,
         const Authorization           &auth):
    th_(th),
    from_(from),
    to_(to),
    srv_(srv),
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
  /** \brief get server configuration part.
   *  \return parameters to connect to SMTP server.
   */
  const Server &getServerConfig(void) const
  {
    return srv_;
  }
  /** \brief get sender's e-mail addresses.
   *  \return e-mail address of the sender.
   */
  const std::string &getSenderAddress(void) const
  {
    return from_;
  }
  /** \brief get recipients e-mail addresses.
   *  \return addresses (e-mails) of recipients.
   */
  const Recipients &getRecipientsAddresses(void) const
  {
    return to_;
  }

private:
  Simple::ThresholdConfig th_;
  std::string             from_;
  Recipients              to_;
  Server                  srv_;
  bool                    useAuth_;
  Authorization           auth_;
}; // class Config

} // namespace Mail
} // namespace Trigger

#endif
