/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_MAIL_CONFIG_HPP_FILE

#include "Trigger/Simple/ThresholdConfig.hpp"

// TODO: implementation
// TODO: comments
// TODO: tests

namespace Trigger
{
namespace Mail
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief create configration description.
   *  \param th threshold configuration - informs when run trigger.
   */
  Config(const Simple::ThresholdConfig &th):
    th_(th)
  {
  }

  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }

private:
  Simple::ThresholdConfig th_;
}; // class Config

} // namespace Mail
} // namespace Trigger

#endif
