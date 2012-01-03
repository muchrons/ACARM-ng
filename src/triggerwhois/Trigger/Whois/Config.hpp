/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_WHOIS_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_WHOIS_CONFIG_HPP_FILE

#include <boost/filesystem.hpp>

#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace Whois
{

class Config
{
public:
  /** \brief create configration description.
   *  \param templateFile path to file containing message template.
   *  \param th           threshold configuration - informs when run trigger.
   */
  Config(const boost::filesystem::path &templateFile, const Simple::ThresholdConfig &th):
    templateFile_(templateFile),
    th_(th)
  {
  }

  /** \brief get path to file containing message template.
   *  \return path to template file.
   */
  const boost::filesystem::path &getTemplateFilePath(void) const
  {
    return templateFile_;
  }

  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }

private:
  boost::filesystem::path templateFile_;
  Simple::ThresholdConfig th_;
}; // class Config

} // namespace Whois
} // namespace Trigger
#endif
