/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE

#include <string>
#include <inttypes.h>
#include <boost/filesystem.hpp>

#include "System/Enum.hpp"
#include "Base/NonEmptyVector.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"
#include "Mail/Config.hpp"

namespace Trigger
{
namespace Mail
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief create configration description, without authorization.
   *  \param th   threshold configuration - informs when run trigger.
   *  \param mc   mail configuration.
   *  \param to   recipients list.
   */
  Config(const Simple::ThresholdConfig    &th,
         const ::Mail::Config             &mc,
         const ::Mail::Config::Recipients &to):
    th_(th),
    mc_(mc),
    to_(to)
  {
  }

  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }

  /** \brief get mail configuration.
   *  \return mail config.
   */
  const ::Mail::Config &getMailConfig(void) const
  {
    return mc_;
  }
  /** \brief get recipients e-mail addresses.
   *  \return addresses (e-mails) of recipients.
   */
  const ::Mail::Config::Recipients &getRecipientsAddresses(void) const
  {
    return to_;
  }

private:
  Simple::ThresholdConfig    th_;
  ::Mail::Config             mc_;
  ::Mail::Config::Recipients to_;
}; // class Config

} // namespace Mail
} // namespace Trigger

#endif
