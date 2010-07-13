/*
 * TriggerConfig.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_TRIGGERCONFIG_HPP_FILE
#define INCLUDE_CONFIGIO_TRIGGERCONFIG_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/Generic/NamedConfig.hpp"

namespace ConfigIO
{

/** \brief class representing configuration of trigger.
 */
class TriggerConfig: public Generic::NamedConfig<TriggerConfig>
{
public:
  /** \brief creates trigger's configuration.
   *  \param type    trigger's type.
   *  \param options options for given trigger.
   */
  TriggerConfig(const TypeName &type,
                const TypeName &name,
                const Options  &options):
    Generic::NamedConfig<TriggerConfig>(type, name, options)
  {
  }
}; // class TriggerConfig

} // namespace ConfigIO

#endif
