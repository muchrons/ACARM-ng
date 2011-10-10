/*
 * TriggerConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_TRIGGERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_TRIGGERCONFIG_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/Generic/Config.hpp"

namespace ConfigIO
{

/** \brief class representing configuration of trigger.
 */
class TriggerConfig: public Generic::Config<TriggerConfig>
{
public:
  /** \brief creates trigger's configuration.
   *  \param type    trigger's type.
   *  \param options options for given trigger.
   */
  TriggerConfig(const TypeName &type,
                const Options  &options):
    Generic::Config<TriggerConfig>(type, options)
  {
  }
  /** \brief creates trigger's configuration with preprocessor.
   *  \param type    trigger's type.
   *  \param options options for given trigger.
   *  \param ppCfg   preprocessor's configuration.
   */
  TriggerConfig(const TypeName             &type,
                const Options              &options,
                const Preprocessor::Config &ppCfg):
    Generic::Config<TriggerConfig>(type, options, ppCfg)
  {
  }

  /** \brief gets trigger's name.
   *  \return trigger's name.
   */
  std::string getTriggerName(void)
  {
    return get("name");
  }
}; // class TriggerConfig

} // namespace ConfigIO

#endif
