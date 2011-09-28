/*
 * Config.hpp
 *
 */
#ifndef INCLUDE_TRIGGER_SNORTSAM_CONFIG_HPP_SNORTSAM
#define INCLUDE_TRIGGER_SNORTSAM_CONFIG_HPP_SNORTSAM

#include "Persistency/IPTypes.hpp"
#include "Persistency/PortNumber.hpp"
#include "Trigger/Simple/ThresholdConfig.hpp"

namespace Trigger
{
namespace SnortSam
{

#if 0
/** \brief module's configuration representation.
 */
class Config: public Persistency::IPTypes<Config>
{
public:
  /** \brief create configration description.
   *  \param th     threshold configuration - informs when run trigger.
   */
  Config(const std::string             &host,
         const Persistency::PortNumber  port,
         const std::string             &key
         const Simple::ThresholdConfig &th):
  {
  }

  /** \brief get destination direcotry to write reports to.
   *  \return path to output direcotry.
   */
  const boost::filesystem::path &getOutputDirectory(void) const
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
  boost::filesystem::path outdir_;
  Simple::ThresholdConfig th_;
}; // class Config
#endif

} // namespace SnortSam
} // namespace Trigger

#endif
