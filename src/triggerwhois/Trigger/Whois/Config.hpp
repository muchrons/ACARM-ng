/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_WHOIS_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_WHOIS_CONFIG_HPP_FILE

#include <boost/filesystem.hpp>

#include "Mail/Config.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace Whois
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief create configration description.
   *  \param templateFile path to file containing message template.
   *  \param th           threshold configuration - informs when run trigger.
   *  \param mc           mail configuration.
   */
  Config(const boost::filesystem::path &templateFile,
         const Simple::ThresholdConfig &th,
         const ::Mail::Config          &mc):
    templateFile_(templateFile),
    th_(th),
    mc_(mc)
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
  /** \brief get mail configuration.
   *  \return mail config.
   */
  const ::Mail::Config &getMailConfig(void) const
  {
    return mc_;
  }


private:
  boost::filesystem::path templateFile_;
  Simple::ThresholdConfig th_;
  ::Mail::Config          mc_;
}; // class Config

} // namespace Whois
} // namespace Trigger
#endif
