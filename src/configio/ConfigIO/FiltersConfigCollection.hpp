/*
 * FiltersConfigCollection.hpp
 *
 */
#ifndef INCLUDE_CONFIGIO_FILTERSCONFIGCOLLECTION_HPP_FILE
#define INCLUDE_CONFIGIO_FILTERSCONFIGCOLLECTION_HPP_FILE

/* public header */

#include <vector>

#include "ConfigIO/FilterConfig.hpp"

namespace ConfigIO
{

/** \brief collection of configurations for given filters. */
typedef std::vector<FilterConfig> FiltersConfigCollection;

} // namespace ConfigIO

#endif
