/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_FILE_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_FILE_CONFIG_HPP_FILE

#include <string>

#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace File
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief create configration description.
   *  \param outdir directory to output reports to.
   *  \param th     threshold configuration - informs when run trigger.
   */
  Config(const std::string &outdir, const Simple::ThresholdConfig &th):
    outdir_(outdir),
    th_(th)
  {
  }

  /** \brief get destination direcotry to write reports to.
   *  \return path to output direcotry.
   */
  const std::string &getOutputDirectory(void) const
  {
    return outdir_;
  }
  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }

private:
  std::string             outdir_;
  Simple::ThresholdConfig th_;
}; // class Config

} // namespace File
} // namespace Trigger

#endif
