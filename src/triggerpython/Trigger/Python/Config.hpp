/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_PYTHON_CONFIG_HPP_FILE
#define INCLUDE_TRIGGER_PYTHON_CONFIG_HPP_FILE

#include <boost/filesystem.hpp>

#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace Python
{

/** \brief whole module's configuration representation.
 */
class Config
{
public:
  /** \brief path in the filesystem. */
  typedef boost::filesystem::path Path;

  /** \brief create configration description, without authorization.
   *  \param th   threshold configuration - informs when run trigger.
   *  \param from sender's e-mail address.
   *  \param to   recipient address.
   *  \param srv  server to connect to.
   */
  Config(const Simple::ThresholdConfig &th,
         const Path                    &path):
    th_(th),
    path_(path)
  {
  }

  /** \brief get thresholds configuration.
   *  \return threshold's config.
   */
  const Simple::ThresholdConfig &getThresholdConfig(void) const
  {
    return th_;
  }
  /** \brief gets script path.
   *  \return path to script with python code.
   */
  const Path &getScriptPath(void) const
  {
    return path_;
  }

private:
  Simple::ThresholdConfig th_;
  Path                    path_;
}; // class Config

} // namespace Python
} // namespace Trigger

#endif
