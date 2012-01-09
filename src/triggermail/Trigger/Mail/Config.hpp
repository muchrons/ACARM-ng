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
   */
  Config(const Simple::ThresholdConfig &th,
         const ::Mail::Config      &mc):
    th_(th),
    mc_(mc)
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

private:
  Simple::ThresholdConfig th_;
  ::Mail::Config      mc_;
}; // class Config

} // namespace Mail
} // namespace Trigger

#endif
