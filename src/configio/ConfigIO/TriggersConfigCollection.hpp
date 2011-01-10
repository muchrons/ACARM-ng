/*
 * TriggersConfigCollection.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_TRIGGERSCONFIGCOLLECTION_HPP_FILE
#define INCLUDE_CONFIGIO_TRIGGERSCONFIGCOLLECTION_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/TriggerConfig.hpp"

namespace ConfigIO
{

/** \brief collection of configurations for given triggers. */
typedef std::vector<TriggerConfig> TriggersConfigCollection;

} // namespace ConfigIO

#endif
