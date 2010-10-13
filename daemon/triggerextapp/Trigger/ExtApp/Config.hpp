/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_EXTAPP_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_EXTAPP_CONFIG_HPP_FILE

#include <boost/filesystem.hpp>

#include "Trigger/Simple/ThresholdConfig.hpp"
#include "Trigger/ExtApp/Exception.hpp"

namespace Trigger
{
namespace ExtApp
{

/** \brief configuration of the trigger
 */
class Config
{
public:
  /** \brief filesystem path. */
  typedef boost::filesystem::path Path;

  /** \brief create configration description.
   *  \param path path to executable to run.
   *  \param th   threshold configuration - informs when run trigger.
   */
  Config(const Path &path, const Simple::ThresholdConfig &th);
  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const;
  /** \brief gets path to application to be executed.
   *  \return path to application.
   */
  const Path &getPath(void) const;

private:
  Simple::ThresholdConfig th_;
  Path                    path_;
}; // class Config

} // namespace ExtApp
} // namespace Trigger

#endif
