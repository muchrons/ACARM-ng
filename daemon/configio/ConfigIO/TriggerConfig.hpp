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

  /** \brief gets trigger's name.
   *  \return trigger's name.
   */
  const std::string getTriggerName(void) const
  {
    return get("name");
  }
}; // class TriggerConfig

} // namespace ConfigIO

#endif
